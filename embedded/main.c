#include <bcm2835.h>
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <float.h>
#include <string.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <sys/time.h>

#define API_COMMENCE_CALL 0
#define API_REGISTER_ALARM 1
#define API_ADD_EVENT 2
#define API_ALARM_CHECK 3
#define API_CLEAR_ALARM 4

//#define PROXY_ENABLE

struct WriteThis {
	char * readptr;
	long sizeleft;
};

struct alarm_info {
	char * id;
	char * alarm;
	int alert;
	int temp;
	int photo;
	int sound;
};

double read_temp(char * path);
char * find_sensor();
void send_call(int call, struct alarm_info info); 
static size_t read_callback(void *ptr, size_t size, size_t nmemb, void *userp);
static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp);

static int arduino_fd;

int main(int argc, char **argv) {
	if(!bcm2835_init())
		return 1;

	bcm2835_gpio_fsel(RPI_GPIO_P1_08, BCM2835_GPIO_FSEL_OUTP);

	char * temp_sensor_path = find_sensor();
	char * mac_address = (char*) malloc(sizeof(char));
	FILE * mac_fp = fopen("/sys/class/net/eth0/address", "r");
	if(mac_fp == NULL) {
		fprintf(stderr, "Unable to read MAC address\n");
		exit(1);
	}

	int mac_size = 0;
	getline(&mac_address, &mac_size, mac_fp);
	*(strstr(mac_address, "\n")) = 0;
	printf("%s\n", mac_address);

	struct alarm_info info;
	info.id = mac_address;
	info.alarm = "home";

	arduino_fd = wiringPiI2CSetup(0x04);
	if(arduino_fd == -1) {
		fprintf(stderr, "Failed to open Arduino I2C (device id %d)\n", 0x04);
		exit(1);
	}

	struct timeval last_update_time;
	struct timeval last_alarm_check_time;
	struct timeval current_time;

	last_update_time.tv_sec = 0;
	last_alarm_check_time.tv_sec = 0;

	double temp = -DBL_MAX;
	curl_global_init(CURL_GLOBAL_ALL);

	while(1) {
		gettimeofday(&current_time, NULL);
		bcm2835_gpio_write(RPI_GPIO_P1_08, HIGH);
		bcm2835_delay(10);

		if(temp_sensor_path != NULL) {
			temp = read_temp(temp_sensor_path);
			if(temp != -DBL_MAX) {
		//		printf("Temperature: %fC\n", temp);
			} else {
		//		printf("Temperature read failed\n");
			}
		}
		
		wiringPiI2CWrite(arduino_fd, 'm');
		int mic_level = 0;
		read(arduino_fd, &mic_level, 2);
		wiringPiI2CWrite(arduino_fd, 'l');
		int light_level = 0;
		read(arduino_fd, &light_level, 2);

		//printf("Mic: %d\nLight: %d\n", mic_level, light_level);

		info.alert = 0;
		info.temp = temp;
		info.photo = light_level;
		info.sound = mic_level;

		if(current_time.tv_sec - last_update_time.tv_sec > 300) {
			send_call(API_ADD_EVENT, info);
			gettimeofday(&last_update_time, NULL);
			printf("Waiting 300 seconds for next update\n");
		}

		if(current_time.tv_sec - last_alarm_check_time.tv_sec > 10) {
			send_call(API_ALARM_CHECK, info);
			gettimeofday(&last_alarm_check_time, NULL);
			printf("Waiting 10 seconds for next alarm check\n");
		}

		bcm2835_gpio_write(RPI_GPIO_P1_08, LOW);

		//wiringPiI2CWrite(arduino_fd, 'b');
	}
	bcm2835_close();
	curl_global_cleanup();
	return 0;
}

char * find_sensor() {
	DIR * dp = opendir("/sys/bus/w1/devices");
	if(dp == NULL) return NULL;
	char * path = NULL;
	struct dirent *ep;
	while(ep = readdir(dp)) {
		if(strstr(ep->d_name, "28-") == ep->d_name) {
			path = (char*) malloc(1024 * sizeof(char));
			strcpy(path, "/sys/bus/w1/devices/");
			strcat(path, ep->d_name);
			strcat(path, "/w1_slave");
			break;
		}
	}
	closedir(dp);
	return path;
}

double read_temp(char * path) {
	FILE * fp = fopen(path, "r");
	if(fp == NULL) return -DBL_MAX;
	char * buf = (char*) malloc(128 * sizeof(char));
	int len = 0;
	ssize_t read = getline(&buf, &len, fp);
	if(strstr(buf, "YES") == NULL) {
		fclose(fp);
		return -DBL_MAX;
	}
	read = getline(&buf, &len, fp);
	char * pos = strstr(buf, "t=") + 2;
	double temp = atoi(pos) / 1000.0;
	fclose(fp);
	free(buf);
	return temp;
}

void send_call(int call, struct alarm_info info) {
	CURL * curl;
	CURLcode res;
	curl = curl_easy_init();
	if(curl) {
		curl_easy_setopt(curl, CURLOPT_POST, 1L);

		#ifdef PROXY_ENABLE
			curl_easy_setopt(curl, CURLOPT_PROXY, "35.2.87.86:8888");
			curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
		#endif

		curl_easy_setopt(curl, CURLOPT_READFUNCTION, read_callback);
		curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

		char * json = (char*) malloc(1024 * sizeof(char));
		char * endpoint = (char*) malloc(512 * sizeof(char));

		struct WriteThis wbmem;
		wbmem.readptr = NULL;
		wbmem.sizeleft = 0;

		switch(call) {
			case API_COMMENCE_CALL:
				sprintf(endpoint, "https://web.engr.illinois.edu/~reese6/smoldr/commenceCall.php?id=%s", info.id);
				curl_easy_setopt(curl, CURLOPT_URL, endpoint);
				sprintf(json, "{\"id\": \"%s\", \"alarm\": \"%s\", \"alert\": %d, \"temp\": %d, \"photo\": %d, \"sound\", %d}", info.id, info.alarm, info.alert, info.temp, info.photo, info.sound);
				break;
			case API_REGISTER_ALARM:
				sprintf(endpoint, "https://web.engr.illinois.edu/~reese6/smoldr/addAlarm.php?id=%s&alarm=%s", info.id, info.alarm);
				curl_easy_setopt(curl, CURLOPT_URL, endpoint); 
				sprintf(json, "{\"id\": \"%s\", \"alarm\": \"%s\"}", info.id, info.alarm);
				break;
			case API_ADD_EVENT:
				sprintf(endpoint, "https://web.engr.illinois.edu/~reese6/smoldr/addEvent.php?id=%s&alert=%d&temp=%d&photo=%d&sound=%d", info.id, info.alert, info.temp, info.photo, info.sound);
				curl_easy_setopt(curl, CURLOPT_URL, endpoint); 
				sprintf(json, "{\"id\": \"%s\", \"alert\": %d, \"temp\": %d, \"photo\": %d, \"sound\", %d}", info.id, info.alert, info.temp, info.photo, info.sound);
				break;
			case API_CLEAR_ALARM:
				sprintf(endpoint, "https://web.engr.illinois.edu/~reese6/smoldr/setAlarm.php?id=%s&on=%d", info.id, 0);
				curl_easy_setopt(curl, CURLOPT_URL, endpoint); 
				sprintf(json, "{\"id\": \"%s\", \"alert\": 0}", info.id);
				break;
			case API_ALARM_CHECK:
				sprintf(endpoint, "https://web.engr.illinois.edu/~reese6/smoldr/alarmCheck.php?id=%s", info.id);
				curl_easy_setopt(curl, CURLOPT_URL, endpoint);
				curl_easy_setopt(curl, CURLOPT_READFUNCTION, read_callback);
				/* send all data to this function  */ 
				curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
				/* we pass our 'chunk' struct to the callback function */ 
				curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&wbmem);
				sprintf(json, "{\"id\", \"%s\"}", info.id);
				break;
		}

		struct WriteThis data;
		data.readptr = json;
		data.sizeleft = (long) strlen(data.readptr);
		
		curl_easy_setopt(curl, CURLOPT_READDATA, &data);

		#ifdef USE_CHUNKED
		    {
				struct curl_slist *chunk = NULL;
						   
				chunk = curl_slist_append(chunk, "Transfer-Encoding: chunked");
				res = curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);
				/* use curl_slist_free_all() after the *perform() call to free this
				list again */ 
			}
		#else
			    /* Set the expected POST size. If you want to POST large amounts of data,
				          consider CURLOPT_POSTFIELDSIZE_LARGE */ 
			    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, data.sizeleft);
		#endif

		#ifdef DISABLE_EXPECT
    	/*
      	Using POST with HTTP 1.1 implies the use of a "Expect: 100-continue"
      	header.  You can disable this header with CURLOPT_HTTPHEADER as usual.
      	NOTE: if you want chunked transfer too, you need to combine these two
      	since you can only set one list of headers with CURLOPT_HTTPHEADER. */ 
 
    	/* A less good option would be to enforce HTTP 1.0, but that might also
       	have other implications. */ 
   		{
      		struct curl_slist *chunk = NULL;
 
      		chunk = curl_slist_append(chunk, "Expect:");
      		res = curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);
      		/* use curl_slist_free_all() after the *perform() call to free this
         	list again */ 
  		}
		#endif
		
		res = curl_easy_perform(curl);
		if(res != CURLE_OK)
			fprintf(stderr, "failed to post to backend: %s\n", curl_easy_strerror(res));
		else if(call == API_ALARM_CHECK) {
			switch(wbmem.readptr[0]) {
				case '1':
					wiringPiI2CWrite(arduino_fd, 'B');
					break;
				case '2':
					wiringPiI2CWrite(arduino_fd, 'b');
					wiringPiI2CWrite(arduino_fd, 'b');
					send_call(API_CLEAR_ALARM, info);
					break;
				default:
					wiringPiI2CWrite(arduino_fd, 'c');
					break;
			}
		}

		curl_easy_cleanup(curl);
		free(json);
		free(endpoint);
	}
}

static size_t read_callback(void *ptr, size_t size, size_t nmemb, void *userp) {
	struct WriteThis *data = (struct WriteThis *) userp;
	if(size*nmemb < 1) return 0;
	if(data->sizeleft) {
		*(char*) ptr = data->readptr[0];
		data->readptr++;
		data->sizeleft--;
		return 1;
	}

	return 0;
}

static size_t
WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
		  size_t realsize = size * nmemb;
		    struct WriteThis *mem = (struct WriteThis *)userp;
			 
			  mem->readptr = realloc(mem->readptr, mem->sizeleft + realsize + 1);
			    if(mem->readptr == NULL) {
						    /* out of memory! */ 
						    printf("not enough memory (realloc returned NULL)\n");
							    return 0;
								  }
				 
				  memcpy(&(mem->readptr[mem->sizeleft]), contents, realsize);
				    mem->sizeleft += realsize;
					  mem->readptr[mem->sizeleft] = 0;
					   
					    return realsize;
}
