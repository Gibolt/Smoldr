//
//  MainTabBar.swift
//  Smoldr
//
//  Created by Andrew Breckenridge on 9/7/14.
//  Copyright (c) 2014 asb. All rights reserved.
//

import UIKit
import Foundation

class MainTabBar: UITabBarController {
    
    func viewDidAppear() {
        if !NSUserDefaults.standardUserDefaults().boolForKey("isLoggedIn") {
            println("you got it son")
        }

    }
}