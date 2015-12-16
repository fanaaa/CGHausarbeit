//
//  main.cpp
//  CGHausarbeit_NardmannZumberovic
//
//  Created by Fana Zumberovic on 14.12.15.
//  Collaborator: Philipp Nardmann
//  Copyright © 2015 HochschuleOsnabrueck. All rights reserved.
//

#include <iostream>
#include "Scene.h"

int main(int argc, const char * argv[]) {
    
    Scene scene = Scene("test.osh");
    scene.parseFile();
    return 0;
}
