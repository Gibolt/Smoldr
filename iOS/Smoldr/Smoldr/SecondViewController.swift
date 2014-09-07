//
//  SecondViewController.swift
//  Smoldr
//
//  Created by Andrew Breckenridge on 9/6/14.
//  Copyright (c) 2014 asb. All rights reserved.
//

import UIKit

class SecondViewController: UITableViewController, UITableViewDelegate, UITableViewDataSource {
    @IBOutlet var historyTableView: UITableView!
    
    override func viewDidLoad() {
        super.viewDidLoad()
        // Do any additional setup after loading the view, typically from a nib.
    }

    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Dispose of any resources that can be recreated.
    }

    // MARK: Data Source
    override func tableView(tableView: UITableView, cellForRowAtIndexPath indexPath: NSIndexPath) -> UITableViewCell {
        var historyTVC: HistoryUITableViewCell = tableView.dequeueReusableCellWithIdentifier("historyCell") as HistoryUITableViewCell
        historyTVC.historyTitle.text = "hello"
        historyTVC.historySubtitle.text = "smaller hello"        
        return historyTVC
    }
    
    override func tableView(tableView: UITableView, numberOfRowsInSection section: Int) -> Int {
        return 5 // TODO: Make array length
    }
    
    
    // MARK: Delegate
    override func tableView(tableView: UITableView, didSelectRowAtIndexPath indexPath: NSIndexPath) {
        
        tableView.deselectRowAtIndexPath(indexPath, animated: true)
    }


}

