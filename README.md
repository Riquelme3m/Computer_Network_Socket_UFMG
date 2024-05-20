# Smart Grid Control System Project

## Introduction
The Smart Grid is a communication and information technology-based system suitable for the generation, supply, and consumption of energy. It incorporates intelligent networks into power plants to collect and manage data effectively, enabling control based on the gathered information. Smart grids utilize bidirectional flow of information to create an automated, widely distributed system with new functionalities, including control, operational efficiency, network resilience, and better integration of renewable technologies.

## Project Overview
A control and automation company has decided to venture into Smart Grid control and needs to develop a pilot project. The project involves creating a network information control capable of managing the state of an electrical grid through a centralized control unit, the Main Terminal Unit (MTU), and a Remote Terminal Unit (RTU), responsible for collecting data from field devices such as sensors and returning it to the MTU through communication protocols. This communication occurs over the Internet.

## System Components
- Main Terminal Unit (MTU): Acts as the server, responsible for managing information flow and controls in the system.
- Remote Terminal Unit (RTU): Acts as the client, collecting data from sensors and communicating with the MTU.
- Sensors: Collect data on electric current, voltage, and energy efficiency.

## Communication
The client program (RTU) collects data from sensors and sends it to the server (MTU), which calculates and stores electrical power along with energy efficiency. The server waits to receive commands from the client to initiate changes, such as turning sensors on/off or updating sensor information.

## Project Tasks
- Develop a client-server network system to simulate interaction between MTU and RTU.
- Implement functionalities such as turning sensors on/off, updating sensor information, and querying sensor data.
- Use POSIX socket API for communication via TCP protocol in C language.

## Objectives
- Implement MTU (server) using socket interface in C language.
- Implement RTU (client) using socket interface in C language.
- Write a project report documenting the implementation and results.

