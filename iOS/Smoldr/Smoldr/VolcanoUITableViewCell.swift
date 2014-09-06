//
//  VolcanoUITableViewCell.swift
//  Smoldr
//
//  Created by Andrew Breckenridge on 9/6/14.
//  Copyright (c) 2014 asb. All rights reserved.
//

import UIKit

class VolcanoUITableViewCell: UITableViewCell, UITableViewDataSource, UITableViewDelegate {

    required init(coder aDecoder: NSCoder) {
        fatalError("init(coder:) has not been implemented")
    }
    
    @IBOutlet var volcanoImageView: UIImageView!
    @IBOutlet var volcanoTitle: UILabel!
    @IBOutlet var volcanoSubtitle: UILabel!
    @IBOutlet var volcanoDatestamp: UILabel!
    
    // MARK: Data Source
    func tableView(tableView: UITableView!, cellForRowAtIndexPath indexPath: NSIndexPath!) -> UITableViewCell! {
        <#code#>
    }
    
    func tableView(tableView: UITableView!, numberOfRowsInSection section: Int) -> Int {
        <#code#>
    }
    
    
    // MARK: Delegate
    func tableView(tableView: UITableView!, didSelectRowAtIndexPath indexPath: NSIndexPath!) {
        <#code#>
    }
}
