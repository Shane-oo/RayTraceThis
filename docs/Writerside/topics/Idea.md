# Idea

## The Problem
People can't afford the hardware or have the skills to develop their own fully PBR Ray Traced scenes. 
A lot of companies (like Constructive Software) want a quick and easy input and output solution, so they can easily 
see a 3D rendering of their set-up and give it to their clients.

## The Solution
A Software as a Service site, where users can provide the resources they want rendered, and have it sent away for
rendering. Done with either the front end interface or via a request-able api.
A fully spec out computer will complete these renderings, so they look as detailed as possible.


## General idea steps
1. User signs up for service, entering in billing details
2. User sets up the scene that they want rendered (either front end or via api), providing things like:
   1. The gltf file of the model
   2. Materials for each shape~~~~
   3. Where and what lights are in the scene
   4. Ability to add more than one model to the scene (decorations for example) and where it should be placed
   5. What they want photos of (where is the camera facing and its position)
   6. etc...
3. Scene is sent of for full rendering
4. User is charged for each rendering a long with the time it takes to download and (potentially store) their resources
5. Once rendering is done it is sent to client (screenshots)
6. User is billed for renders

## Ideas
- Front end will be a user drag and drop set up using three js to give the user the general idea of what their final 
    render will look like
- A user can select available models and textures to complete their scenes or provide their own personal ones.
- Each camera and the position its facing will be one render, user cans et up multiple cameras, directions and angles



![RenderingExample.jpg](RenderingExample.jpg)