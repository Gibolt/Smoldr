//
//  FirstViewController.swift
//  Smoldr
//
//  Created by Andrew Breckenridge on 9/6/14.
//  Copyright (c) 2014 asb. All rights reserved.
//

import UIKit

class FirstViewController: UIViewController {
                            
    @IBOutlet var volcanoTableView: UITableView!

    override func viewDidLoad() {
        manualSegueCheck()
    }

    func manualSegueCheck() {
        if !NSUserDefaults.standardUserDefaults().boolForKey("isLoggedIn") {
            // go to login
            //self.presentViewController(LoginSignupTabBarController(), animated: true, completion: nil)
        }
    }

    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Dispose of any resources that can be recreated.
    }
    
    // MARK: Data Source
    func tableView(tableView: UITableView!, cellForRowAtIndexPath indexPath: NSIndexPath!) -> UITableViewCell! {
        var volcanoUITableViewCell: VolcanoUITableViewCell = tableView.dequeueReusableCellWithIdentifier("volcanoCell") as VolcanoUITableViewCell
        volcanoUITableViewCell.volcanoTitle.text = "hello"
        volcanoUITableViewCell.volcanoSubtitle.text = "smaller hello"
        volcanoUITableViewCell.volcanoImageView.image = UIImage(named: "first")
        volcanoUITableViewCell.backgroundColor = UIColor(red: 10, green: 10, blue: 10, alpha: 1.0)
        
        return volcanoUITableViewCell
    }
    
    func tableView(tableView: UITableView!, numberOfRowsInSection section: Int) -> Int {
        return 5 // TODO: Make array length
    }
    
    
    // MARK: Deleoverride gate
    func tableView(tableView: UITableView!, didSelectRowAtIndexPath indexPath: NSIndexPath!) {
        
        tableView.deselectRowAtIndexPath(indexPath, animated: true)
    }

    
}

