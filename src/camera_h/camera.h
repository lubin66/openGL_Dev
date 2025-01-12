#ifndef CAMERA_H
#define CAMERA_H
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<iostream>
#include<vector>

enum Camera_Movement{
 FORWARD,
 BACKWARD,
 LEFT,
 RIGHT
};

//Default camera values
const float YAW         = -90.0f;
const float PITCH       = 0.0f;
const float SPEED       = 2.5f;
const float SENSITIVITY = 0.01f;
const float ZOOM	= 45.0f;

class Camera
{
   public:
      //Camera Attributes
      glm::vec3 Postion;
      glm::vec3 Front;
      glm::vec3 Up;
      glm::vec3 Right;
      glm::vec3 WorldUp;
      //Euler Angles
      float Yaw;
      float Pitch;
      
      //Camera options
      float MovementSpeed;
      float MouseSensitivity;
      float Zoom;
      
      //Constructor with vector
      Camera(glm::vec3 postion = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH): Front(glm::vec3(0.0f, 0.0f, -1.0f)),MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
      {
         Postion = postion;
         WorldUp  = up;
         Yaw      = yaw;
         Pitch    = pitch;
         updateCameraVectors();
      }
      
      //Constructor with scalar values
      Camera(float posX, float posY, float posZ,float upX,float upY,float upZ,float yaw, float pitch): Front(glm::vec3(0.0f, 0.0f, -1.0f)),MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY),Zoom(ZOOM)
     {
       Postion = glm::vec3(posX, posY, posZ);
       WorldUp  = glm::vec3(upX, upY, upZ);
       Yaw      = yaw;
       Pitch    = pitch;
       updateCameraVectors();
     }
     
     //Return the View matrix calculated using Euler Angles and the lookat matrix
     glm::mat4 GetViewMatrix()
     {
         return glm::lookAt(Postion, Postion+Front, Up);
     }
     //Process input received from any keyboard-like input system
     void ProcessKeyBoard(Camera_Movement direction, float deltaTime)
     {
         float velocity = MovementSpeed * deltaTime;
         if(direction == FORWARD)
            Postion += Front * velocity;
         if(direction == BACKWARD)
            Postion -= Front * velocity;
         if(direction == LEFT)
            Postion -= Right * velocity;
         if(direction == RIGHT)
            Postion += Right * velocity;
         //Postion.y = 0.0f;  <-- this one-liner keeps the user at the ground level (xz plane)
     }
     //Process Mouse event
     void ProcessMouseMovement(float xoffset, float yoffset,bool constrainPitch = true)
     {
         xoffset *= MouseSensitivity;
         yoffset *= MouseSensitivity;
         
         Yaw    += xoffset;
         Pitch  += yoffset;
         std::cout<<"Yaw"<<Yaw<<"\n"; 
         if(constrainPitch)
         {
            if(Pitch > 89.0f)
               Pitch = 89.0f;
            if(Pitch < -89.0f)
               Pitch = -89.0f;
         }
         if (Yaw < -85.0f)
         {
             Yaw = -85.0f;
         }
         if(Yaw > 85.0f)
         {
             Yaw = 85.0f;
         }
         //update Front ,Right Up Vectors  using to updat Euler angles
         updateCameraVectors();
         
     }

     void ProcessMouseScroll(float yoffset)
     {
         if(Zoom >= 1.0f & Zoom <= 45.0f)
            Zoom -= yoffset;
         if(Zoom <= 1.0f)
            Zoom = 1.0f;
         if(Zoom >= 45.0f)
            Zoom = 45.0f;
     }
         
 private:
     void updateCameraVectors()
     {
         glm::vec3 front;
         front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
         front.y = sin(glm::radians(Pitch));
         front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
         Front = glm::normalize(front);
         Right = glm::normalize(glm::cross(Front, WorldUp));
         Up    = glm::normalize(glm::cross(Right, Front));
     }

};
#endif
