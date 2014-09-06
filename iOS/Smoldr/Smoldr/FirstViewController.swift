//
//  FirstViewController.swift
//  Smoldr
//
//  Created by Andrew Breckenridge on 9/5/14.
//  Copyright (c) 2014 asb. All rights reserved.
//

import UIKit

class FirstViewController: UITableViewController, UITableViewDelegate, UITableViewDataSource {

    
    @IBOutlet var volcanoTableView: UITableView!
                            
    override func viewDidLoad() {
        super.viewDidLoad()
        // Do any additional setup after loading the view, typically from a nib.
    }

    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Dispose of any resources that can be recreated.
    }

    // MARK: Data Source
    override func tableView(tableView: UITableView!, cellForRowAtIndexPath indexPath: NSIndexPath!) -> UITableViewCell! {
        var volcanoUITableViewCell: VolcanoUITableViewCell = tableView.dequeueReusableCellWithIdentifier("volcanoCell") as VolcanoUITableViewCell
        volcanoUITableViewCell.volcanoTitle.text = "hello"
        volcanoUITableViewCell.volcanoSubtitle.text = "smaller hello"
        volcanoUITableViewCell.volcanoImageView.image = UIImage(named: "first")
        volcanoUITableViewCell.volcanoDatestamp.text = "4:09pm"
        
        return volcanoUITableViewCell
    }
    
    override func tableView(tableView: UITableView!, numberOfRowsInSection section: Int) -> Int {
        return 5 // TODO: Make array length
    }
    
    
    // MARK: Deleoverride gate
    override func tableView(tableView: UITableView!, didSelectRowAtIndexPath indexPath: NSIndexPath!) {
        
        tableView.deselectRowAtIndexPath(indexPath, animated: true)
    }


}

