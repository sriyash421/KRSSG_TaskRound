#include "naobehavior.h"
#include "../rvdraw/rvdraw.h"

extern int agentBodyType;

/*
 * Real game beaming.
 * Filling params x y angle
 */
void NaoBehavior::beam( double& beamX, double& beamY, double& beamAngle ) {
    beamX = -HALF_FIELD_X + worldModel->getUNum();
    beamY = 0;
    beamAngle = 0;
}


SkillType NaoBehavior::selectSkill() {
    // My position and angle
    //cout << worldModel->getUNum() << ": " << worldModel->getMyPosition() << ",\t" << worldModel->getMyAngDeg() << "\n";

    // Position of the ball
    //cout << worldModel->getBall() << "\n";

    // Example usage of the roboviz drawing system and RVSender in rvdraw.cc.
    // Agents draw the position of where they think the ball is
    // Also see example in naobahevior.cc for drawing agent position and
    // orientation.
    /*
    worldModel->getRVSender()->clear(); // erases drawings from previous cycle
    worldModel->getRVSender()->drawPoint("ball", ball.getX(), ball.getY(), 10.0f, RVSender::MAGENTA);
    */

    // ### Demo Behaviors ###

    // Walk in different directions
    // return goToTargetRelative(VecPosition(1,0,0), 0); // Forward
    // return goToTargetRelative(VecPosition(-1,0,0), 0); // Backward
    // return goToTargetRelative(VecPosition(0,1,0), 0); // Left
    // return goToTargetRelative(VecPosition(0,-1,0), 0); // Right
    // return goToTargetRelative(VecPosition(1,1,0), 0); // Diagonal
    // return goToTargetRelative(VecPosition(0,1,0), 90); // Turn counter-clockwise
    // return goToTargetRelative(VecPdosition(0,-1,0), -90); // Turn clockwise
    // return goToTargetRelative(VecPosition(1,0,0), 15); // Circle

    // Walk to the ball
    // return goToTarget(ball);

    // Turn in place to face ball
    // double distance, angle;
    // getTargetDistanceAndAngle(ball, distance, angle);
    // if (abs(angle) > 10) {
    //   return goToTargetRelative(VecPosition(), angle);
    // } else {
    //   return SKILL_STAND;
    // }

    // Walk to ball while always facing forward
    //return goToTargetRelative(worldModel->g2l(ball), -worldModel->getMyAngDeg());

    // Dribble ball toward opponent's goal
    //return kickBall(KICK_DRIBBLE, VecPosition(HALF_FIELD_X, 0, 0));

    // Kick ball toward opponent's goal
    //return kickBall(KICK_FORWARD, VecPosition(HALF_FIELD_X, 0, 0)); // Basic kick
    //return kickBall(KICK_IK, VecPosition(HALF_FIELD_X, 0, 0)); // IK kick

    // Just stand in place
    //return SKILL_STAND;

    // Demo behavior where players form a rotating circle and kick the ball
    // back and forth
    //return demoKickingCircle();


    VecPosition me=worldModel->getMyPosition();
    VecPosition start   = VecPosition(0.2,0,0);
    VecPosition center = VecPosition(HALF_FIELD_X/2.0, 0, 0);
    VecPosition target = VecPosition(2*HALF_FIELD_X/3.0, 0, 0);

    int opponentClosestToBall=-1;
    double closestDistanceToBall=10000;

    for(int i=WO_OPPONENT1;i<WO_OPPONENT1+4;i++)
    {
        VecPosition temp;
        int playerNum = i - WO_OPPONENT1 + 1;
        WorldObject* opponent = worldModel->getWorldObject( i );
        if(opponent->validPosition)
        {
            temp =opponent->pos;
        }
        else 
            continue;

        double distanceToBall=temp.getDistanceTo(ball);
        //double distanceToMe=temp.getDistanceTo(me);

        if (distanceToBall < closestDistanceToBall)
        {
            opponentClosestToBall = playerNum;
            closestDistanceToBall = distanceToBall;
        }
    }

    WorldObject* opponent=worldModel->getWorldObject(opponentClosestToBall+WO_OPPONENT1-1);
    VecPosition myOpp=opponent->pos;



    // if(me.getDistanceTo(start) <= 0)
    // {
    //     if(me.getDistanceTo(ball)<0.5)
    //     {
    //         cout << "Going to target" <<endl;
    //         target = collisionAvoidance(true /*teammate*/, true/*opponent*/, false/*ball*/, 1proximity thresh, 1/*collision thresh*/, target, true/*keepDistance*/);
    //         goToTarget(target);
    //     }
    //     else
    //     {
    //         cout << "Going to start" <<endl;
    //         goToTarget(start);
    //     }
    // }

    if(me.getDistanceTo(ball)<closestDistanceToBall  && ball.getX()>0.5*HALF_FIELD_X && me.getDistanceTo(ball)>5 && closestDistanceToBall<2)
    {
        cout <<"Tackling"<<endl;
        target=ball+VecPosition(0.5,0,0);
        goToTarget(target);
        double distance, angle;
        getTargetDistanceAndAngle(ball, distance, angle);
        return goToTargetRelative(VecPosition(), angle);
    }

    if(me.getDistanceTo(ball) > 0.5)
    {
        cout << "Going to ball"<<endl;
        goToTarget(ball);
    }

    if(abs(myOpp.getY()-me.getY()) < 2 && me.getDistanceTo(ball) <0.5 && myOpp.getX()-me.getX() < 3 && myOpp.getX() -me.getX() >0 && me.getDistanceTo(ball) < 0.5)
     {
        int direction =1;
        if(myOpp.getY()>me.getY())
            direction=-1;
        if(me.getX()>0.9*HALF_FIELD_X)
        { 
           if(ball.getX()>0)
                direction=1;
            else
                direction=-1;

        // if(me.getX()>0.9*HALF_FIELD_X)
        // { 
        //    if(me.getY()>right_post.getY())
        //         target=right_post+VecPosition(-2,0,0);
        //     else if(me.getY()<left_post.getY())
        //         target=left_post+VecPosition(2,0,0);
        //     else   
        //         target=myOpp+VecPosition(10,4*direction,0);
        //     return kickBall(KICK_IK, target);
        // }
        }
        cout <<"Dribbling enemy"<<endl;
        target=myOpp+VecPosition(10,4*direction,0);
        //target = collisionAvoidance(true /*teammate*/, true/*opponent*/, false/*ball*/, 2/*proximity thresh*/, 1/*collision thresh*/, target, true/*keepDistance*/);
        return kickBall(KICK_DRIBBLE, target);
    }
        
    if( me.getX()>0.8*HALF_FIELD_X && me.getDistanceTo(ball) < 0.5)// && myOpp.getDistanceTo(me)<4 )
    {
        double a=computeKickCost(VecPosition(HALF_FIELD_X,0,0),SKILL_KICK_IK_0_RIGHT_LEG);
        if(a>2.0)
            return goToTargetRelative(worldModel->g2l(ball), -worldModel->getMyAngDeg());
        cout << "Scoring goal"<<endl;
        return kickBall(KICK_IK, VecPosition(HALF_FIELD_X, 0, 0));        

    }
    else if(me.getX() > 2*HALF_FIELD_X/3.0 && me.getDistanceTo(ball) < 0.5)
    {
        if(worldModel->distanceToOppGoal(me) < 2)
        {
            cout << "Shooting" <<endl;
            double a=computeKickCost(VecPosition(HALF_FIELD_X,0,0),SKILL_KICK_IK_0_RIGHT_LEG);
            if(a>2.0)
                return goToTargetRelative(worldModel->g2l(ball), -worldModel->getMyAngDeg());
            return kickBall(KICK_IK, VecPosition(HALF_FIELD_X, 0, 0));       
        }
        else
        {
            cout <<"Approaching goal"<<endl;
            return kickBall(KICK_DRIBBLE, VecPosition(HALF_FIELD_X, 0, 0));
        }
    }
    if(me.getX()>HALF_FIELD_X/3.0 && me.getDistanceTo(ball) < 0.5)
    {
        cout << "Middle Third"<<endl;
        target=VecPosition(HALF_FIELD_X,0,0);
        target = collisionAvoidance(true /*teammate*/, true/*opponent*/, false/*ball*/, 1/*proximity thresh*/, 0.5/*collision thresh*/, target, true/*keepDistance*/);
        return kickBall(KICK_DRIBBLE, VecPosition(HALF_FIELD_X, 0, 0));
    }

    if(me.getX()<HALF_FIELD_X/3.0 && me.getDistanceTo(ball) < 0.5)
    {
        cout << "Initial Third"<<endl;
        target=VecPosition(HALF_FIELD_X*0.5,HALF_FIELD_Y*0.25,0);
        target = collisionAvoidance(true /*teammate*/, true/*opponent*/, false/*ball*/, 1/*proximity thresh*/, 0.5/*collision thresh*/, target, true/*keepDistance*/);
        return kickBall(KICK_DRIBBLE, VecPosition(HALF_FIELD_X, 0, 0));   
    }
                                                                                        

    
 
    // target = collisionAvoidance(true /*teammate*/, true/*opponent*/, false/*ball*/, 2/*proximity thresh*/, 1/*collision thresh*/, target, true/*keepDistance*/);

    

    // if(closestDistanceToBall > HALF_FIELD_X*0.2)
    // {    
    //    if(me.getDistanceTo(ball)<0.5)
    //     {
    //         if(me.getDistanceTo(start) > 2*HALF_FIELD_X/3.0)
    //         {
    //             cout << "Scoring goal"<<endl;
    //             return kickBall(KICK_IK, VecPosition(HALF_FIELD_X, 0, 0));
    //         }
    //         else
    //         {
    //             cout << "Going to target, Avoiding obstacles"<<endl;
    //             target = collisionAvoidance(true /*teammate*/, true/*opponent*/, false/*ball*/, 2/*proximity thresh*/, 1/*collision thresh*/, target, true/*keepDistance*/);
    //             goToTarget(target);
    //         }
    //    }
    //     else
    //     {
    //         cout << "Going to ball, Avoiding obstacles"<<endl;
    //         target = collisionAvoidance(true teammate, true/*opponent*/, false/*ball*/, 2/*proximity thresh*/, 1/*collision thresh*/, target, true/*keepDistance*/);
    //         return goToTargetRelative(worldModel->g2l(ball), -worldModel->getMyAngDeg());
    //     }   
    // }
    // else
    // {
    //     target=center;
    //     target = collisionAvoidance(true /*teammate*/, true/*opponent*/, false/*ball*/, 2/*proximity thresh*/, 1/*collision thresh*/, target, true/*keepDistance*/);
    //     cout << "Avoiding obstacles"<<endl;
    //     if(me.getDistanceTo(ball)<0.5)
    //     {
    //         cout << "Going to ball"<<endl;
    //         goToTarget(ball);
    //     }
    //     if(me.getDistanceTo(start) > 2*HALF_FIELD_X/3.0)
    //     {
    //             return kickBall(KICK_IK, VecPosition(HALF_FIELD_X, 0, 0));
    //     }
    //     else

    //     goToTarget(target);
       
    // }

    // cout <<"Base Condition"<<endl;
    // goToTarget(ball);
    // return kickBall(KICK_DRIBBLE, VecPosition(HALF_FIELD_X*1.02, 0, 0));

}


/*
 * Demo behavior where players form a rotating circle and kick the ball
 * back and forth
 */
// SkillType NaoBehavior::demoKickingCircle() {
//     // Parameters for circle
//     VecPosition center = VecPosition(-HALF_FIELD_X/2.0, 0, 0);
//     double circleRadius = 5.0;
//     double rotateRate = 2.5;

//     // Find closest player to ball
//     int playerClosestToBall = -1;
//     double closestDistanceToBall = 10000;
//     for(int i = WO_TEAMMATE1; i < WO_TEAMMATE1+NUM_AGENTS; ++i) {
//         VecPosition temp;
//         int playerNum = i - WO_TEAMMATE1 + 1;
//         if (worldModel->getUNum() == playerNum) {
//             // This is us
//             temp = worldModel->getMyPosition();
//         } else {
//             WorldObject* teammate = worldModel->getWorldObject( i );
//             if (teammate->validPosition) {
//                 temp = teammate->pos;
//             } else {
//                 continue;
//             }
//         }
//         temp.setZ(0);

//         double distanceToBall = temp.getDistanceTo(ball);
//         if (distanceToBall < closestDistanceToBall) {
//             playerClosestToBall = playerNum;
//             closestDistanceToBall = distanceToBall;
//         }
//     }

//     if (playerClosestToBall == worldModel->getUNum()) {
//         // Have closest player kick the ball toward the center
//         return kickBall(KICK_FORWARD, center);
//     } else {
//         // Move to circle position around center and face the center
//         VecPosition localCenter = worldModel->g2l(center);
//         SIM::AngDeg localCenterAngle = atan2Deg(localCenter.getY(), localCenter.getX());

//         // Our desired target position on the circle
//         // Compute target based on uniform number, rotate rate, and time
//         VecPosition target = center + VecPosition(circleRadius,0,0).rotateAboutZ(360.0/(NUM_AGENTS-1)*(worldModel->getUNum()-(worldModel->getUNum() > playerClosestToBall ? 1 : 0)) + worldModel->getTime()*rotateRate);

//         // Adjust target to not be too close to teammates or the ball
//         target = collisionAvoidance(true /*teammate*/, false/*opponent*/, true/*ball*/, 1/*proximity thresh*/, .5/*collision thresh*/, target, true/*keepDistance*/);

//         if (me.getDistanceTo(target) < .25 && abs(localCenterAngle) <= 10) {
//             // Close enough to desired position and orientation so just stand
//             return SKILL_STAND;
//         } else if (me.getDistanceTo(target) < .5) {
//             // Close to desired position so start turning to face center
//             return goToTargetRelative(worldModel->g2l(target), localCenterAngle);
//         } else {
//             // Move toward target location
//             return goToTarget(target);
//         }
//     }
// }



