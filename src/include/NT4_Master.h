/*! \mainpage NT4 User Manual

	The goal of the system to give users a system they can download and play with, no in between, assuming a good binary, otherwise it is compile then use. On windows a CMake build script is included so they can compile and play in one click. When they have it set up they can start playing with the batch file interface on Windows.On Linux you are assumed to be savvy enough to figure out the compilation, use, and interfacing. The engine envisioned as a "redstone component" in operation, I / O and interface meant to be used in a network of components, in this case scripts. It is obviously more complex than a simple redstone repeater, but the idea of encapsulation in such a manner remains.

	\section overview Overview
	- The system operates as a server, it runs continuously in a loop until shutdown
	- It has several files, input.ssv, config.ssv, control_panel.ssv, control_panel_flag.ssv, control_panel_complete, and output.ssv
	- All I / O is done through these space separated volumes for simplicity
	- The ML engine has your typical public interface for C++ OOP classes
	- This interface of the ML is then "piped" to and from these text files.The user may store a command followed by the arguments and the system will compile this into a function call to the ML engine.
	- The interface contains an "interpreter" that allows for "running" the config and control_panel files as sequential system calls to the ML engine, we'll call this interpreter eval()
	- The reason for this is for integration into shell scripts, and to be able to easily prototype networks, workflows, and configurations using the live shell engine, and then when ready you go implement the design by transliterating the code from text to C++.
	- This system is the highest abstraction of the engine, expected to be the lowest barrier of entry so it is presented as the 'face' of the engine, though the C++ pure interface has advantages to those who can C++.
*/


/** \addtogroup NT4_Namespace
 *  @{
 */

 /** \namespace NT4
	 \brief This is the namespace which encapsulates the NT4 Neural-Net Engine and the interfaces.
	 
 */
//The NeuroTox 4 library.
namespace NT4
{

	//Used in the CAN & the Node Network for backprop.
	#include "NT4/c_IO.h"

	#include "NT4/c_Linked_List_Handler.h"

	#include "NT4/c_Node.h"

	#include "NT4/c_Trace.h"

	#include "NT4/c_Fractal_Tree.h"

	#include "NT4/c_Node_Network.h"

	#include "NT4/c_Charging_Array.h" 
	//#include "NT4/c_Backpropagation_Station
	// 
	//Both the Charging_Array & Backpropagation_Station have to have members to handle each type of CAN structure.
	#include "NT4/c_CAN.h"
	#include "NT4/c_CAN_Many_To_One.h"
	#include "NT4/c_CAN_1D_Pyramid.h"
	#include "NT4/c_CAN_2D_Pyramid.h"
	#include "NT4/c_CAN_3D_Pyramid.h"
	
	#include "NT4/c_Construct.h"
	#include "NT4/c_Construct_API.h"
	#include "NT4/c_Construct_Text_Server.h"
}
/** @}*/