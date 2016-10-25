#include <ros.h>
#include <ros/time.h>
#include <tf/transform_broadcaster.h>

ros::NodeHandle  nh;

geometry_msgs::TransformStamped t;
tf::TransformBroadcaster broadcaster;

// defines pins numbers
const int stepPin = 3; 
const int dirPin = 4; 
const int stallPin = 10;
const int upPin = 9;
const int downPin = 8;
const int scanPin = 11;

char base_link[] = "/lidar_platform";
char odom[] = "/lidar_origin";
float height = 0.1;
float stepHeight = 0.001;
int numberOfSteps = 0;
int delayLength = 800;

void goUP()
{
        digitalWrite(stallPin,LOW); //enable the controller
        digitalWrite(dirPin,HIGH); // Enables the motor to move in a particular direction
        // Makes 600 pulses
        digitalWrite(stepPin,HIGH); 
        delayMicroseconds(delayLength); 
        digitalWrite(stepPin,LOW); 
        delayMicroseconds(delayLength);
}

void goDOWN()
{
        digitalWrite(stallPin,LOW); //enable the controller
        digitalWrite(dirPin,LOW); //Changes the rotations direction
        // Makes 600 pulses
        digitalWrite(stepPin,HIGH);
        delayMicroseconds(delayLength);
        digitalWrite(stepPin,LOW);
        delayMicroseconds(delayLength);
}

void publish()
{
  t.header.frame_id = odom;
  t.child_frame_id = base_link;
  t.transform.translation.x = 0.0;
  t.transform.translation.y = 0.0; 
  t.transform.translation.z = height; 
  t.transform.rotation.x = 0.0;
  t.transform.rotation.y = 0.0; 
  t.transform.rotation.z = 0.0; 
  t.transform.rotation.w = 1.0;  
  t.header.stamp = nh.now();
  broadcaster.sendTransform(t);
  nh.spinOnce();
}

void setup()
{
  // make the pushbuttons pins inputs:
  pinMode(upPin, INPUT);
  pinMode(downPin, INPUT);
  // Sets the two pins as Outputs
  pinMode(stepPin,OUTPUT); 
  pinMode(dirPin,OUTPUT);
  nh.initNode();
  broadcaster.init(nh);
}

void loop()
{
  // read the state of the pushbutton value:
  int up = digitalRead(upPin);
  int down = digitalRead(downPin);
  int scan = digitalRead(scanPin);


      if (up == 1)
      {
        goUP();
      }
      else if (down == 1)
      {
        goDOWN();
      }


      else if (scan == 1)
      {
        for (height = 0.11; height < 0.66; height += stepHeight)
        {
          numberOfSteps = stepHeight/(0.00125/200.0);
          for (int x = 0; x < numberOfSteps; x++)
          {
            goUP();
          }
          publish();
        }
                for (height = 0.66; height > 0.11; height -= stepHeight)
        {
          numberOfSteps = stepHeight/(0.00125/200.0);
          for (int x = 0; x < numberOfSteps; x++)
          {
            goDOWN();
          }
          publish();
        }
      }
      
      else{
        digitalWrite(stallPin,HIGH); //disable the controller
      }

//  while (height < 0.8)
//  {
//    height += 0.01;
//    publish();
//    delay(10);
//  }
//  while (height > 0.1)
//  {
//    height -= 0.01;
//    publish();
//    delay(10);
//  }
}

