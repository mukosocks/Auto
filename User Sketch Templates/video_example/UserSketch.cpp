#include "UserSketch.h"

UserSketch::UserSketch() {
    
}

//--------------------------------------------------------------

void UserSketch::setup() {
    
    //Setup Screen Dimensions (width, ratio w:h, door-projection speed calib, monitor mode)
    screen = new Screen(640, 1.33, 0.625, false);
    
    content = "hawk"; // type in content package name
    
    l0.loadMovie("movies/" + content + "/l0.mov");
    r0.loadMovie("movies/" + content + "/r0.mov");
    l1.loadMovie("movies/" + content + "/l1.mov");
    r1.loadMovie("movies/" + content + "/r1.mov");
    l2.loadMovie("movies/" + content + "/l2.mov");
    r2.loadMovie("movies/" + content + "/r2.mov");
    l3.loadMovie("movies/" + content + "/l3.mov");
    r3.loadMovie("movies/" + content + "/r3.mov");
    l4.loadMovie("movies/" + content + "/l4.mov");
    r4.loadMovie("movies/" + content + "/r4.mov");
    l5.loadMovie("movies/" + content + "/l5.mov");
    r5.loadMovie("movies/" + content + "/r5.mov");
    l6.loadMovie("movies/" + content + "/l6.mov");
    r6.loadMovie("movies/" + content + "/r6.mov");
    
    lf.loadMovie("movies/" + content + "/l6.mov");
    rf.loadMovie("movies/" + content + "/r6.mov");
    
    l0.play();
    r0.play();
    l1.play();
    r1.play();
    l2.play();
    r2.play();
    l3.play();
    r3.play();
    l4.play();
    r4.play();
    l5.play();
    r5.play();
    l6.play();
    r6.play();
    
    lf.play();
    rf.play();
    
}

//--------------------------------------------------------------
void UserSketch::update() {
    
    // video frame calculations
    
    // if State Change detected . . .
    
    if (doorstate != lastdoorstate) {
        
        // initialize video frames  // create fuction to calculate initial frame by pass door position
        
        tempstate = doorstate;
        temppct = ((doorposition*100)/maxgap);
        
        //cout << "gap percentage: " << temppct;
        
        switch (tempstate) {
            case 0: // fully closed
                tempframe = (l0.getTotalNumFrames() * temppct) / 100;
                //cout << " start frame: " << tempframe << endl;
                l0.setFrame(tempframe); // tempframe: frame at position, 0: beginning of video
                r0.setFrame(tempframe);
                break;
            case 1: // slowly closing
                tempframe = (l1.getTotalNumFrames() * (100 - temppct)) / 100;
                //cout << " start frame: " << tempframe << endl;
                l1.setFrame(tempframe);
                r1.setFrame(tempframe);
                break;
            case 2: // closing
                tempframe = (l2.getTotalNumFrames() * (100 - temppct)) / 100;
                //cout << " start frame: " << tempframe << endl;
                l2.setFrame(tempframe);
                r2.setFrame(tempframe);
                break;
            case 3: // stopped
                tempframe = (l3.getTotalNumFrames() * temppct) / 100;
                //cout << " start frame: " << tempframe << endl;
                l3.setFrame(tempframe);
                r3.setFrame(tempframe);
                break;
            case 4: // opening
                tempframe = (l4.getTotalNumFrames() * temppct) / 100;
                //cout << " start frame: " << tempframe << endl;
                l4.setFrame(tempframe);
                r4.setFrame(tempframe);
                break;
            case 5: // slowly opening
                tempframe = (l5.getTotalNumFrames() * temppct) / 100;
                //cout << " start frame: " << tempframe << endl;
                l5.setFrame(tempframe);
                r5.setFrame(tempframe);
                break;
            case 6: // fully opened
                tempframe = (l6.getTotalNumFrames() * temppct) / 100;
                //cout << " start frame: " << tempframe << endl;
                l6.setFrame(tempframe);
                r6.setFrame(tempframe);
                break;
            default:
                break;
        }
        
    } else {
        
        doorstatechanged = false;
        
    }
    
    temppct = NULL;
    lastdoorstate = doorstate;
    
    l0.update();
    r0.update();
    l1.update();
    r1.update();
    l2.update();
    r2.update();
    l3.update();
    r3.update();
    l4.update();
    r4.update();
    l5.update();
    r5.update();
    l6.update();
    r6.update();
    
    lf.update();
    rf.update();
    
}

//--------------------------------------------------------------
void UserSketch::draw() {
    
    ofBackground(0);
    
    lf.draw(fixpos_l, 0, screenwidth, screenheight);
    rf.draw(fixpos_r, 0, screenwidth, screenheight);
    
    // design by video
    
    switch (doorstate) {
            
        case 0:
            l0.draw(screenpos_l,0,screenwidth,screenheight);
            r0.draw(screenpos_r,0,screenwidth,screenheight);
            break;
        case 1:
            l1.draw(screenpos_l,0,screenwidth,screenheight);
            r2.draw(screenpos_r,0,screenwidth,screenheight);
            break;
        case 2:
            l2.draw(screenpos_l,0,screenwidth,screenheight);
            r2.draw(screenpos_r,0,screenwidth,screenheight);
            break;
        case 3:
            l3.draw(screenpos_l,0,screenwidth,screenheight);
            r3.draw(screenpos_r,0,screenwidth,screenheight);
            break;
        case 4:
            l4.draw(screenpos_l,0,screenwidth,screenheight);
            r4.draw(screenpos_r,0,screenwidth,screenheight);
            break;
        case 5:
            l5.draw(screenpos_l,0,screenwidth,screenheight);
            r5.draw(screenpos_r,0,screenwidth,screenheight);
            break;
        case 6:
            l6.draw(screenpos_l,0,screenwidth,screenheight);
            r6.draw(screenpos_r,0,screenwidth,screenheight);
            break;
        default:
            l0.draw(screenpos_l,0,screenwidth,screenheight);
            r0.draw(screenpos_r,0,screenwidth,screenheight);
            break;
            
    }
    
    
}