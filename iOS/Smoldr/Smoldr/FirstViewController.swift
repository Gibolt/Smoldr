//
//  FirstViewController.swift
//  Smoldr
//
//  Created by Andrew Breckenridge on 9/5/14.
//  Copyright (c) 2014 asb. All rights reserved.
//

import UIKit

class FirstViewController: UITableViewController {

    required init(coder aDecoder: NSCoder) {
        fatalError("init(coder:) has not been implemented")
    }
    
    @IBOutlet var volcanoTableView: UITableView!
                            
    override func viewDidLoad() {
        super.viewDidLoad()
        // Do any additional setup after loading the view, typically from a nib.
    }

    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Dispose of any resources that can be recreated.
    }


}

