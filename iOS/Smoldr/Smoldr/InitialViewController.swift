//
//  InitialViewController.swift
//  Smoldr
//
//  Created by Andrew Breckenridge on 9/6/14.
//  Copyright (c) 2014 asb. All rights reserved.
//

import UIKit

class InitialViewController: UIViewController {
    
    override func viewDidLoad() {
        println("viewDidLoad")
        if NSUserDefaults.standardUserDefaults().boolForKey("isLoggedIn") {
            // go to login
            self.performSegueWithIdentifier("segueToMainPage", sender: nil)
            println("mainpaging")
        } else {
            // go to home fire view
            self.performSegueWithIdentifier("ShowLoginSignupPage", sender: nil)
            println("loginpaging")
        }
    }
}
