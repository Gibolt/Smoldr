//
//  ThirdViewController.swift
//  Smoldr
//
//  Created by Andrew Breckenridge on 9/6/14.
//  Copyright (c) 2014 asb. All rights reserved.
//

import UIKit

class ThirdViewController: UITableViewController {
    
    @IBOutlet var settingsTable: UITableView!
    var data = ["911 Settings", "Family Group Setings", "Outer Group Settings", "Notification Settings", "Account Information", "Add A Sensor", "Log Out"]
    
    override func viewDidLoad() {
        super.viewDidLoad()
        // Do any additional setup after loading the view, typically from a nib.
    }
    
    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Dispose of any resources that can be recreated.
    }
    
    // MARK: Data Source
    func settingsTable(tableView: UITableView!, cellForRowAtIndexPath indexPath: NSIndexPath!) -> UITableViewCell! {
        var volcanoUITableViewCell: VolcanoUITableViewCell = settingsTable.dequeueReusableCellWithIdentifier("settingsID") as VolcanoUITableViewCell
        volcanoUITableViewCell.volcanoTitle.text = data[indexPath.row]
        
        return volcanoUITableViewCell
    }
    
    override func numberOfSectionsInTableView(tableView: UITableView) -> Int {
        return 2
    }
    
    override func tableView(tableView: UITableView, numberOfRowsInSection section: Int) -> Int {
        return data.count
    }
    
    
    // MARK: Deleoverride gate
    override func tableView(tableView: UITableView, didSelectRowAtIndexPath indexPath: NSIndexPath) {
        
        tableView.deselectRowAtIndexPath(indexPath, animated: true)
    }
    
}
