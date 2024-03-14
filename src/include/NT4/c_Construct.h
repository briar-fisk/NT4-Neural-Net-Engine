/** \addtogroup Construct
 *  @{
 */

 /** \class c_Construct
	 \brief This class encapsulates and manipulates the Neural Network Engine directly while providing a public interface.

	 Inside the construct you'll find the raw C++. The interface brings highly granular control from the sub-classes to the surface. On this interface the other ones are built.

 */

//The construct encapsulates the node network, the CANs, I/O, granulation filter, uinterface, actuator interface, and I/O tables.
//The result is that after setting up the network the user can use it like a black box.
class c_Construct
{
public:

	//The shared node network.
	c_Node_Network Nodes;

	//The names of each assembly within the construct.
	std::string* Assembly_Names;

	//These are the files each assembly uses for input, defaults to "Input.ssv"
	std::string* Assembly_Input_Files;

	//The file for each assembly to use as output.
	std::string* Assembly_Output_Files;

	//For each assembly we have an index in the State_Tree[], CAN[], etc.
	int Assembly_Count;

	//This holds the array of CAN scaffold structures.
	//c_Base_CAN is a base class.
	c_Base_CAN** CAN; 

	//This array keeps track of what type each CAN is.
	std::string* CAN_Type;

	c_Construct()
	{
		Assembly_Count = 0; //Variables dependent on this value: Assembly_Names, CAN
		Assembly_Names = NULL;
		Assembly_Input_Files = NULL;
		Assembly_Output_Files = NULL;
		CAN = NULL;
		CAN_Type = NULL;
	}

	//I need to refactor this jfc
	int add_Assembly(std::string p_Assembly_Name)
	{
		std::string* tmp_Names;
		tmp_Names = new std::string[Assembly_Count];

		std::string* tmp_Input_Files;
		tmp_Input_Files = new std::string[Assembly_Count];

		std::string* tmp_Output_Files;
		tmp_Output_Files = new std::string[Assembly_Count];
		
		c_Base_CAN** tmp_CAN;
		tmp_CAN = new c_Base_CAN * [Assembly_Count];

		std::string* tmp_CAN_Type;
		tmp_CAN_Type = new std::string[Assembly_Count];

		for (int cou_Index = 0; cou_Index < Assembly_Count; cou_Index++)
		{
			tmp_Names[cou_Index] = Assembly_Names[cou_Index];

			tmp_Input_Files[cou_Index] = Assembly_Input_Files[cou_Index];
			tmp_Output_Files[cou_Index] = Assembly_Output_Files[cou_Index];

			tmp_CAN_Type[cou_Index] = CAN_Type[cou_Index];
			tmp_CAN[cou_Index] = CAN[cou_Index];
		}

		if (Assembly_Names != NULL) { delete[] Assembly_Names; }
		if (Assembly_Input_Files != NULL) { delete[] Assembly_Input_Files; }
		if (Assembly_Output_Files != NULL) { delete[] Assembly_Output_Files; }
		if (CAN != NULL) { delete[] CAN; }
		if (CAN_Type != NULL) { delete[] CAN_Type; }

		Assembly_Names = new std::string[Assembly_Count + 1];
		Assembly_Input_Files = new std::string[Assembly_Count + 1];
		Assembly_Output_Files = new std::string[Assembly_Count + 1];
		CAN = new c_Base_CAN * [Assembly_Count + 1];
		CAN_Type = new std::string[Assembly_Count + 1];


		for (int cou_Index = 0; cou_Index < Assembly_Count; cou_Index++)
		{
			Assembly_Names[cou_Index] = tmp_Names[cou_Index];

			Assembly_Input_Files[cou_Index] = tmp_Input_Files[cou_Index];
			Assembly_Output_Files[cou_Index] = tmp_Output_Files[cou_Index];

			CAN[cou_Index] = tmp_CAN[cou_Index];
			tmp_CAN[cou_Index] = NULL;

			CAN_Type[cou_Index] = tmp_CAN_Type[cou_Index];
		}

		delete[] tmp_Names; tmp_Names = NULL;
		delete[] tmp_Input_Files; tmp_Input_Files = NULL;
		delete[] tmp_Output_Files; tmp_Output_Files = NULL;
		delete[] tmp_CAN; tmp_CAN = NULL;
		delete[] tmp_CAN_Type; tmp_CAN_Type = NULL;

		Assembly_Names[Assembly_Count] = p_Assembly_Name;

		Assembly_Input_Files[Assembly_Count] = "Input\\" + p_Assembly_Name + ".Input.ssv";
		Assembly_Output_Files[Assembly_Count] = "Output\\" + p_Assembly_Name + ".Output.ssv";

		//The CAN is made later because there are different subtypes.
		CAN[Assembly_Count] = NULL;

		CAN_Type[Assembly_Count] = "NONE";

		Assembly_Count++;

		//Return the index of the newly created assembly.
		return Assembly_Count - 1;
	}


	//==--- DIRECT_HOOK ---==//
	//This doesn't make sense to run through the API
	c_Node* get_Node_Ref_By_NID(uint64_t p_NID)
	{
		return Nodes.get_Node_Ref_By_NID(p_NID);
	}

	





//      ---==================---
//     ---====================---
//    ---======================---
//   ---========================---
//  ---==   NT4 Deep Control   ==---
//   ---========================---
//    ---======================---
//     ---====================---
//      ---==================---
  
  
  
//    ---==========---
//   ---============---
//  ---==   Node   ==---
//   ---============---
//    ---==========---

	//==--- DIRECT_HOOK ---==//
    //(0: State), (1: Branch), (2: Treetop), (3: State/Treetop)
	void set_Type(c_Node * p_Node, uint8_t p_Type)
	{
		if (p_Node != NULL)
		{
			p_Node->set_Type(p_Type);
		}
	}


	//==--- DIRECT_HOOK ---==//
	//Adds an axon to the axon list at the given index, if the index doesn't exist then exist it with resize_Axon_Hillocks()
	void add_Axon_Index(c_Node* p_Node, c_Node* p_Axon, int p_Index)
	{
		if (p_Node != NULL)
		{
			p_Node->add_Axon_Index(p_Axon, p_Index);
		}
	}


	//==--- DIRECT_HOOK ---==//
    //Sets the dendrites of the node.
    //This assumes the node has no dendrites yet, if it does you be dangling and jangling
	void set_Dendrites(c_Node* p_Node, int p_Count, c_Node** p_Dendrites)
	{
		if (p_Node != NULL)
		{
			p_Node->set_Dendrites(p_Dendrites, p_Count);
		}
	}


	//==--- DIRECT_HOOK ---==//
    //Searches the axons to see if an upper tier connection exists.
    //This is always called from the first leg, that is why we separate _F from normal.
	c_Node* does_Upper_Tier_Connection_Exist(int p_Count, c_Node** p_Nodes)
	{
		return Nodes.does_Upper_Tier_Connection_Exist(p_Nodes, p_Count);
	}


	//==--- DIRECT_HOOK ---==//
    //Checks if the given node matches a dendrite on the right leg.
	bool does_Lower_Connection_Exist(c_Node * p_Node, int p_Count, c_Node** p_Nodes)
	{
		if (p_Node != NULL)
		{
			return p_Node->does_Lower_Connection_Exist(p_Nodes, p_Count);
		}
		return 0;
	}



	//==--- DIRECT_HOOK ---==//
    //Binds a node to a quanta of data, the state of the input.
	void bind_State(c_Node * p_Node, uint64_t p_State)
	{
		if (p_Node != NULL)
		{
			p_Node->bind_State(p_State);
		}
	}



	//==--- DIRECT_HOOK ---==//
    //Initiates a backpropagation that outputs the pattern represented by this node.
	void bp_O(c_Node * p_Node)
	{
		if (p_Node != NULL)
		{
			p_Node->bp_O();
		}
	}



	//==--- DIRECT_HOOK ---==//
	//The CAN handles this for backpropagating a trace into a given CAN input. "gather_Given_Trace(uint64_t p_NID)"
	void bp_Trace_O(c_Node * p_Node, c_Linked_List_Handler* p_LL)
	{
		if ((p_Node != NULL) && (p_LL != NULL))
		{
			p_Node->bp_Trace_O(p_LL);
		}
	}



	//==--- DIRECT_HOOK ---==//
	//Outputs the ugly raw info dump for the node.
	void output_Node_Raw(c_Node * p_Node)
	{
		if (p_Node != NULL)
		{
			p_Node->output_Node_Raw();
		}
	}



	//==--- DIRECT_HOOK ---==//
	//Casts the node address to char() and outputs it.
	void output_Node_Char(c_Node * p_Node)
	{
		if (p_Node != NULL)
		{
			p_Node->output_Node_Char();
		}
	}


	
  
  
//    ---==================---
//   ---====================---
//  ---==   Node_Network   ==---
//   ---====================---
//    ---==================---
  
	//==--- DIRECT_HOOK ---==//
	//---==  register_New_Construct  ==---//
	int register_New_Construct()
	{
		return Nodes.register_New_Construct();
	}

  
	//==--- DIRECT_HOOK ---==//
	//Creates a new node and adds it to the fractal tree.
	//Each node is stored as a link in a linked list.
	c_Node* new_Node()
	{
		return Nodes.new_Node();
	}
  

	//==--- DIRECT_HOOK ---==//
	//Creates a new node, then adds it to the state tree.
	//Assumes the construct is already registered so the index is valid.
	c_Node* new_State_Node(int p_Assembly, double p_State)
	{
		return Nodes.new_State_Node(p_Assembly, p_State);
	}
  

	//==--- DIRECT_HOOK ---==//
	//Creates a connection between nodes.
	//p_To forms dendritic connections to p_From, and on p_From you have the axonic connections.
	void create_Connections(c_Node* p_To, int p_Count, c_Node** p_From)
	{
		Nodes.create_Connections(p_To, p_From, p_Count);
	}
  

	//==--- DIRECT_HOOK ---==//
	//Checks if an upper tier node exists.
	c_Node* does_Upper_Tier_Connection_Exist_Network(int p_Count, c_Node** p_Legs)
	{
		return Nodes.does_Upper_Tier_Connection_Exist(p_Legs, p_Count);
	}
  

	//==--- DIRECT_HOOK ---==//
	//Gets an upper tier node based on the given legs. Will create it if not found and give it the type 1.
	c_Node* get_Upper_Tier_Node(int p_Count, c_Node** p_Legs)
	{
		return Nodes.get_Upper_Tier_Node(p_Legs, p_Count, 1);
	}

  
	//==--- DIRECT_HOOK ---==//
	//If a state node exists in the given construct index then return it.
	//Otherwise return NULL.
	//This assumes the [Index] is valid
	c_Node* does_State_Node_Exist(int p_Assembly, uint64_t p_State)
	{
		return Nodes.does_State_Node_Exist(p_Assembly, p_State);
	}
  

	//==--- DIRECT_HOOK ---==//
	//Checks to see if a node in the given assembly is bound to the given state, if not the node is created.
	c_Node* get_State_Node(int p_Assembly, uint64_t p_State)
	{
		return Nodes.get_State_Node(p_Assembly, p_State);
	}
  

	//==--- DIRECT_HOOK ---==//
	//Iterates through every node and outputs their bp_O()
	void output_BP()
	{
		Nodes.output_BP();
	}

  
	//==--- DIRECT_HOOK ---==//
	//Finds given NID and outputs the bp_O()
	void output_BP_NID(int p_NID)
	{
		Nodes.output_BP_NID(p_NID);
	}

  
	//---==  DIRECT_HOOK  ==---//
	//Outputs all of the nodes as raw.
	void output_Node_Network()
	{
		Nodes.output_Raw();
	}



	//    ---=========---
	//   ---===========---
	//  ---==   CAN   ==---
	//   ---===========---
	//    ---=========---

	//==--- DIRECT_HOOK ---==//
	//This encodes the p_Input data, if the nodes aren't found they are created, used for training.
	void encode(int p_Assembly, int p_Depth = 0, uint64_t* p_Input = NULL)
	{
		CAN[p_Assembly]->encode(p_Input, p_Depth);
	}


	//==--- DIRECT_HOOK ---==//
	//This sets up the scaffold as encode does, but it doesn't create nodes if they aren't found, they remain NULL in the scaffold, this we call a NULLCAN
	//Used for querying the network, you input, fill the NULLCAN, charge the network, then gather the outputs.
	//Suggested for use before encoding (if using learning mode and not locked_to_initial_training_mode) otherwise it will also find the current trace as the perfect match.
	void query(int p_Assembly, uint64_t* p_Input = NULL, int p_Depth = 0, int p_Charging_Style = -1, int p_Leg = 0, int* p_Legs = NULL)
	{
		CAN[p_Assembly]->query(p_Input, p_Depth, p_Charging_Style, p_Leg, p_Legs);
	}


	//==--- DIRECT_HOOK ---==//
	//This allows for passing unordered sets of nodes
	void submit_Set(int p_Assembly, int p_Depth, uint64_t* p_Input)
	{
		CAN[p_Assembly]->submit_Set(p_Input, p_Depth);
	}


	//==--- DIRECT_HOOK ---==//
	//Gets the treetop node for a given assembly.
	//This doesn't make sense to create an API for.
	//This returns the treetop node at a given index, for most structures this will be a single node, but for those like stiched-base networks with a treetop node count equal to the input node count then you can access them by index.
	c_Node* get_Treetop(int p_Assembly)
	{
		return (CAN[p_Assembly]->get_Treetop());
	}


	//==--- DIRECT_HOOK ---==//
	//Gets the treetop node for a given assembly.
	uint64_t get_Treetop_NID(int p_Assembly)
	{
		if (CAN[p_Assembly]->get_Treetop() != NULL)
		{
			return (CAN[p_Assembly]->get_Treetop())->NID;
		}
		return 0;
	}


	//==--- DIRECT_HOOK ---==//
	//Gets the treetop node for a given assembly.
	uint64_t get_Treetop_NID_At_Given_Index(int p_Assembly, int p_Index)
	{
		if (CAN[p_Assembly]->get_Treetop(p_Index) != NULL)
		{
			return (CAN[p_Assembly]->get_Treetop(p_Index))->NID;
		}
		return 0;
	}


	//==--- DIRECT_HOOK ---==//
	//Gets a single trace from a given node. Puts it into the output.
	void gather_Given_Trace(int p_Assembly, uint64_t p_NID)
	{
		CAN[p_Assembly]->gather_Given_Trace(p_NID);
		
		gather_Output(p_Assembly);
	}


	//==--- DIRECT_HOOK ---==//
	//Gets a single trace from a given node. Puts it into the output.
	void gather_Given_Trace_uint(int p_Assembly, uint64_t p_NID)
	{
		CAN[p_Assembly]->gather_Given_Trace(p_NID);
		
		gather_Output_uint(p_Assembly);
	}


	//==--- DIRECT_HOOK ---==//
	//Gathers all the traces as it says.
	void gather_All_Traces(int p_Assembly)
	{
		CAN[p_Assembly]->gather_All_Traces();

		gather_Output(p_Assembly);
	}


	//==--- DIRECT_HOOK ---==//
	//Gathers all the traces as it says.
	void gather_All_Traces_uint(int p_Assembly)
	{
		CAN[p_Assembly]->gather_All_Traces();

		gather_Output_uint(p_Assembly);
	}


	//==--- DIRECT_HOOK ---==//
	//Wipe the input array.
	void reset_Input(int p_Assembly)
	{
		CAN[p_Assembly]->reset_Input();
	}


	//==--- DIRECT_HOOK ---==//
	//Associate the CAN with a network from which to draw nodes.
	//This doesn't make sense to wrap in the API.
	void set_NNet(int p_Assembly, c_Node_Network* p_NNet)
	{
		CAN[p_Assembly]->set_NNet(p_NNet);
	}


	//==--- DIRECT_HOOK ---==//
	//Sets the index for the state_Node_Tree in the c_Node_Network::State_Nodes[]
	void set_State_Nodes_Index(int p_Assembly, int p_Index)
	{
		CAN[p_Assembly]->set_State_Nodes_Index(p_Index);
	}


	//==--- DIRECT_HOOK ---==//
	//Sets the input to the given uint64_t array.
	//The input array is 1D.
	void set_Input(int p_Assembly, int p_Input_Depth, uint64_t* p_Input)
	{
		CAN[p_Assembly]->set_Input(p_Input, p_Input_Depth);
	}

	//==--- DIRECT_HOOK ---==//
	//Sets the input to the given uint64_t array.
	//The input array is 2D.
	void set_2D_Input(int p_Assembly, int p_X_Depth, int p_Y_Depth, uint64_t** p_Input)
	{
		CAN[p_Assembly]->set_2D_Input(p_Input, p_X_Depth, p_Y_Depth);
	}

	//==--- DIRECT_HOOK ---==//
	//Sets the input to the given uint64_t array.
	//The input array is 3D.
	void set_3D_Input(int p_Assembly, int p_X_Depth, int p_Y_Depth, int p_Z_Depth, uint64_t*** p_Input)
	{
		CAN[p_Assembly]->set_3D_Input(p_Input, p_X_Depth, p_Y_Depth, p_Z_Depth);
	}


	//==--- DIRECT_HOOK ---==//
	//This is used for setting the input array to reflect a sequence of characters.
	void set_Input_String(int p_Assembly, std::string p_Input)
	{
		CAN[p_Assembly]->set_Input_String(p_Input);
	}


	//==--- DIRECT_HOOK ---==//
	//Outputs the scaffold as addresses.
	void output_Scaffold(int p_Assembly)
	{
		CAN[p_Assembly]->output_Scaffold();
	}


	//==--- DIRECT_HOOK ---==//	
	//==--- CLI_HOOK ---==//	
	//    ---==  output_input [ASSEMBLY_ID]  ==---
	//		Outputs the input of the given assembly to the console.
	//Outputs the input for the Assembly.
	void output_Input(int p_Assembly)
	{
		CAN[p_Assembly]->output_Input();
	}


	//==--- DIRECT_HOOK ---==//	
	//==--- CLI_HOOK ---==//	
	//    ---==  output_input [ASSEMBLY_ID]  ==---
	//		Outputs the input of the given assembly to the console.
	//Outputs the input for the Assembly.
	void output_Input_uint(int p_Assembly)
	{
		CAN[p_Assembly]->output_Input(1);
	}


	//==--- DIRECT_HOOK ---==//
	//The output trace set is output.
	void output_Output(int p_Assembly)
	{
		CAN[p_Assembly]->output_Output(0);
	}


	//==--- DIRECT_HOOK ---==//
	//The output trace set is output.
	void output_Output_uint(int p_Assembly)
	{
		CAN[p_Assembly]->output_Output(1);
	}


	//==--- DIRECT_HOOK ---==//
	//Each address is typecast to a char to give a pseudo-unique look to each node. For monke brain.
	void output_Scaffold_Char(int p_Assembly)
	{
		CAN[p_Assembly]->output_Scaffold_Char();
	}





	//      ---====================---
	//     ---======================---
	//    ---========================---
	//   ---==========================---
	//  ---==   NT4 specific hooks   ==---
	//   ---==========================---
	//    ---========================---
	//     ---======================---
	//      ---====================---



	//    ---======================================---
	//   ---========================================---
	//  ---==   Used to register new assemblies.   ==---
	//   ---========================================---
	//    ---======================================---

		//    ---==  register_assembly [ASSEMBLY_TYPE] [ASSEMBLY_NAME]  ==---
		//p_Type is the type of CAN to declare. 
		// "Many_To_One" - The I/O tier has every node connected to a single upper tier node.
	int register_Assembly(std::string p_Type, std::string p_Assembly_Name)
	{
		int tmp_Assembly_ID = add_Assembly(p_Assembly_Name);
		std::cout << "\n <Register_Assembly> Name: ->" << p_Assembly_Name << "<- | Type: ->" << p_Type << "<- | ID: ->" << tmp_Assembly_ID << "<- ";

		if (p_Type == "Many_To_One")
		{
			std::cout << "\n      -->Created Many_To_One CAN";

			CAN[tmp_Assembly_ID] = new c_CAN_Many_To_One;

			//Make sure we set the node network for the CAN.
			CAN[tmp_Assembly_ID]->set_NNet(&Nodes);

			CAN[tmp_Assembly_ID]->State_Nodes_Index = Nodes.register_New_Construct();

			CAN_Type[tmp_Assembly_ID] = "Many_To_One";
		}
		if (p_Type == "1D_Pyramid")
		{
			std::cout << "\n      -->Created 1D_Pyramid CAN";

			CAN[tmp_Assembly_ID] = new c_CAN_1D_Pyramid;

			//Make sure we set the node network for the CAN.
			CAN[tmp_Assembly_ID]->set_NNet(&Nodes);

			CAN[tmp_Assembly_ID]->State_Nodes_Index = Nodes.register_New_Construct();

			CAN_Type[tmp_Assembly_ID] = "1D_Pyramid";
		}
		if (p_Type == "2D_Pyramid")
		{
			std::cout << "\n      -->Created 2D_Pyramid CAN";

			CAN[tmp_Assembly_ID] = new c_CAN_2D_Pyramid;

			//Make sure we set the node network for the CAN.
			CAN[tmp_Assembly_ID]->set_NNet(&Nodes);

			CAN[tmp_Assembly_ID]->State_Nodes_Index = Nodes.register_New_Construct();

			CAN_Type[tmp_Assembly_ID] = "2D_Pyramid";
		}
		if (p_Type == "3D_Pyramid")
		{
			std::cout << "\n      -->Created 3D_Pyramid CAN";

			CAN[tmp_Assembly_ID] = new c_CAN_3D_Pyramid;

			//Make sure we set the node network for the CAN.
			CAN[tmp_Assembly_ID]->set_NNet(&Nodes);

			CAN[tmp_Assembly_ID]->State_Nodes_Index = Nodes.register_New_Construct();

			CAN_Type[tmp_Assembly_ID] = "3D_Pyramid";
		}

		return tmp_Assembly_ID;
	}




	//    ---=====================---
	//   ---=======================---
	//  ---==   Input handling.   ==---
	//   ---=======================---
	//    ---=====================---

//    ---==  load_input  ==---
	int load_Input(int p_Assembly)
	{
		std::ifstream InputFile(Assembly_Input_Files[p_Assembly]);

		std::string tmp_Input_Full = "";
		std::string tmp_In = "";
		int tmp_Count = 0;

		if (InputFile.is_open())
		{
			while (!InputFile.eof())
			{
				InputFile >> tmp_In;
				std::cout << "\n - [ " << tmp_Count << " ]: " << tmp_In;
				tmp_Count++;

				if (tmp_Input_Full != "") { tmp_Input_Full = tmp_Input_Full + " " + tmp_In; }
				if (tmp_Input_Full == "") { tmp_Input_Full = tmp_In; }

				//Neuralman.output_Input();
			}

			std::cout << "\n Input.ssv contents: " << tmp_Input_Full << "\n";

			//set_Input_1D_string(int p_Assembly, std::string p_Input)

			set_Input(p_Assembly, tmp_Input_Full);

			return 1;
		}
		else
		{
			std::cerr << "\n Unable to open Input.ssv for set_Input_1D_string ...\n";

			return 0;
		}

		return 1;
	}
	
//    ---==  load_input  ==---
	int load_Input_uint(int p_Assembly)
	{
		std::ifstream InputFile(Assembly_Input_Files[p_Assembly]);

		int tmp_Count = 0;
		int tmp_Current = 0;
		uint64_t* tmp_Input = NULL;

		if (InputFile.is_open())
		{
			//Get the count:
			if (!InputFile.eof())
			{
				InputFile >> tmp_Count;
			}

			std::cout << "\n Count: " << tmp_Count;

			if (tmp_Count > 0) 
			{
				tmp_Input = new uint64_t[tmp_Count];

				for (int cou_Index = 0; cou_Index < tmp_Count; cou_Index++)
				{
					tmp_Input[cou_Index] = 0;
				}

				while (!InputFile.eof())
				{
					InputFile >> tmp_Input[tmp_Current];

					std::cout << "\n - [ " << tmp_Current << " ]: " << tmp_Input[tmp_Current];
					tmp_Current++;
				}
			}

			set_Input_uint(p_Assembly, tmp_Count, tmp_Input);

			if (tmp_Input != NULL) { delete[] tmp_Input; tmp_Input = NULL; }

			return 1;
		}
		else
		{
			std::cerr << "\n Unable to open Input.ssv for set_Input_1D_string ...\n";

			return 0;
		}

		return 1;
	}

	//This function moves a uint from a given construct input index to the output of a given construct by appending it. This allows you to take the backpropagated MSC, Chrono, etc pattern of treetops and append each treetop to the output file of the given construct. So you can then run 
	//Comment left here in case I come back to it. Delete if something has superceded it and this comment will not be potentially needed.

	//    ---==  set_input [ASSEMBLY_ID] [INPUT_STRING]  ==---
		//Set the value to the passed 1D string of uint64_t
	void set_Input(int p_Assembly, std::string p_Input)
	{
		CAN[p_Assembly]->set_Input_String(p_Input);
	}

	//    ---==  set_input_uint [ASSEMBLY_ID] [ARRAY_DEPTH] [UINT_ARRAY]  ==---
		//Set the value to the passed 1D string of uint64_t
	void set_Input_uint(int p_Assembly, int p_Depth, uint64_t* p_Input)
	{
		CAN[p_Assembly]->set_Input(p_Input, p_Depth);
	}



	//    ---==========================================================================---
	//   ---============================================================================---
	//  ---==   Different ways of gathering nodes, individually, and the entire set.   ==---
	//   ---============================================================================---
	//    ---==========================================================================---

	//    ---==  gather_given_node [ASSEMBLY_ID] [NID]  ==---
	//		Writes the given node's data down in the Assembly_Output_Files[p_Assembly] file.
	void gather_Given_Node(int p_Assembly, uint64_t p_NID)
	{
		std::ofstream tmp_Output_File;
		c_Node* tmp_Node = NULL;

		tmp_Node = Nodes.get_Node_Ref_By_NID(p_NID);

		tmp_Output_File.open(Assembly_Output_Files[p_Assembly], std::ios::app);

		write_Node_To_File(p_Assembly, &tmp_Output_File, tmp_Node);

		tmp_Output_File.close();
	}

	//		Writes the given node's data down in the Assembly_Output_Files[p_Assembly] file.
	void gather_Given_Node_uint(int p_Assembly, uint64_t p_NID)
	{
		std::ofstream tmp_Output_File;
		c_Node* tmp_Node = NULL;

		tmp_Node = Nodes.get_Node_Ref_By_NID(p_NID);

		tmp_Output_File.open(Assembly_Output_Files[p_Assembly], std::ios::app);

		write_Node_To_File(p_Assembly, &tmp_Output_File, tmp_Node, 1);

		tmp_Output_File.close();
	}

	//    ---==  gather_all_nodes [ASSEMBLY_ID]  ==---
	//		This writes the entire network to the Assembly_Output_Files[p_Assembly] file. Note, the output patterns are treated as character.
		//It uses the passed assembly to output the nodes by putting it into the output of that assembly, then into the file.
	void gather_All_Nodes(int p_Assembly)
	{
		std::ofstream tmp_Output_File;

		c_Node* tmp_Node;
		tmp_Node = Nodes.Root;

		tmp_Output_File.open(Assembly_Output_Files[p_Assembly], std::ios::app);

		while (tmp_Node != NULL)
		{
			tmp_Output_File << "\n";

			write_Node_To_File(p_Assembly, &tmp_Output_File, tmp_Node);

			tmp_Node = tmp_Node->Next;
		}

		tmp_Output_File.close();
	}

	//    ---==  gather_all_nodes_uint [ASSEMBLY_ID]  ==---
	//		This writes the entire network to the Assembly_Output_Files[p_Assembly] file. Note, the output patterns are treated as uint.
	void gather_All_Nodes_uint(int p_Assembly)
	{
		std::ofstream tmp_Output_File;

		c_Node* tmp_Node;
		tmp_Node = Nodes.Root;

		tmp_Output_File.open(Assembly_Output_Files[p_Assembly], std::ios::app);

		while (tmp_Node != NULL)
		{
			tmp_Output_File << "\n";

			write_Node_To_File(p_Assembly, &tmp_Output_File, tmp_Node, 1);

			tmp_Node = tmp_Node->Next;
		}

		tmp_Output_File.close();
	}


	//    ---==================================================================---
	//   ---====================================================================---
	//  ---==   The output of a given assembly is read into the output file.   ==---
	//   ---====================================================================---
	//    ---==================================================================---

		//==--- DIRECT_HOOK ---==//
	void output_Trace_To_File(std::ofstream* p_SF, c_Trace* p_Trace, int p_Output_Type)
	{
		std::cout << " NID: " << p_Trace->Treetop->NID;
		std::cout << " Charge: " << p_Trace->Charge;
		std::cout << " RC: " << p_Trace->RC;
		std::cout << " Depth: " << p_Trace->Depth;

		*p_SF << "\n";

		*p_SF << p_Trace->Treetop->NID;
		*p_SF << " " << p_Trace->Charge;
		*p_SF << " " << p_Trace->RC;
		*p_SF << " " << p_Trace->Depth;

		*p_SF << " ";
		std::string tmp_In = "";

		if (p_Trace->Depth > 0)
		{
			if (p_Output_Type == 0)
			{
				*p_SF << char(p_Trace->Pattern[0]);
				std::cout << "  " << char(p_Trace->Pattern[0]);
			}
			if (p_Output_Type == 1)
			{
				*p_SF << p_Trace->Pattern[0];
				std::cout << "  " << p_Trace->Pattern[0];
			}
		}

		for (int cou_Index = 1; cou_Index < p_Trace->Depth; cou_Index++)
		{
			if (p_Output_Type == 0)
			{
				*p_SF << char(p_Trace->Pattern[cou_Index]);
				std::cout << " " << char(p_Trace->Pattern[cou_Index]);
			}
			if (p_Output_Type == 1)
			{
				*p_SF << " " << p_Trace->Pattern[cou_Index];
				std::cout << " " << p_Trace->Pattern[cou_Index];
			}

		}
	}

	//==--- DIRECT_HOOK ---==//
	//Character output for this one, the default, always string.
	//p_Output_Type: 0 = string, 1 = uint64_t
	void output_Output_To_File(int p_Assembly, int p_Output_Type = 0)
	{
		std::ofstream tmp_Output_File;

		tmp_Output_File.open(Assembly_Output_Files[p_Assembly], std::ios::app);

		// Check if the flag file exists and can be opened
		if (tmp_Output_File.is_open())
		{
			//tmp_Output_File << CAN[p_Assembly]->Output_Depth;

			//For every trace write the info to the file
			for (int cou_Trace = 0; cou_Trace < CAN[p_Assembly]->Output_Depth; cou_Trace++)
			{
				std::cout << "\n Trace [" << cou_Trace << "]";

				output_Trace_To_File(&tmp_Output_File, &(CAN[p_Assembly]->Output[cou_Trace]), p_Output_Type);
			}
		}
		tmp_Output_File.close();
	}

	//    ---==  gather_output [ASSEMBLY_ID]  ==---
	//		Takes every trace in the given Assemblies output trace array and writes them to the Assembly_Output_Files[p_Assembly] file, note the output state patterns are treated as char.
	void gather_Output(int p_Assembly)
	{
		output_Output_To_File(p_Assembly);
	}

	//    ---==  gather_output_uint [ASSEMBLY_ID]  ==---
	//		Takes every trace in the given Assemblies output trace array and writes them to the Assembly_Output_Files[p_Assembly] file, note the output state patterns are treated as uint.
	void gather_Output_uint(int p_Assembly)
	{
		output_Output_To_File(p_Assembly, 1);
	}


	//    ---=============================---
	//   ---===============================---
	//  ---==   Treetop node gathering.   ==---
	//   ---===============================---
	//    ---=============================---


		//==--- DIRECT_HOOK ---==//
	void write_Node_To_File(int p_Assembly, std::ofstream* p_SF, c_Node* p_Node, int p_Output_Type = 0)
	{
		if ((p_SF == NULL) || (p_Node == NULL)) { return; }

		// Check if the flag file exists and can be opened
		if (p_SF->is_open())
		{
			*p_SF << p_Node->NID;
			*p_SF << " ";
			*p_SF << p_Node->RC;
			*p_SF << " ";
			*p_SF << p_Node->Dendrite_Count;
			*p_SF << " ";
			for (int cou_D = 0; cou_D < p_Node->Dendrite_Count; cou_D++)
			{
				*p_SF << p_Node->Dendrites[cou_D]->NID;
				*p_SF << " ";
			}
			*p_SF << p_Node->Axon_Hillock_Count;
			*p_SF << " ";
			for (int cou_H = 0; cou_H < p_Node->Axon_Hillock_Count; cou_H++)
			{
				*p_SF << p_Node->Axon_Count[cou_H];
				*p_SF << " ";
				for (int cou_A = 0; cou_A < p_Node->Axon_Count[cou_H]; cou_A++)
				{
					*p_SF << p_Node->Axons[cou_H][cou_A]->NID;
					*p_SF << " ";
				}
			}

			CAN[p_Assembly]->gather_Given_Trace(p_Node->NID);

			if (CAN[p_Assembly]->Output_Depth > 0)
			{
				//The single node should only generate one trace which will be stored in the [0] index.

				*p_SF << CAN[p_Assembly]->Output[0].Depth;
				*p_SF << " ";
				for (int cou_Index = 0; cou_Index < CAN[p_Assembly]->Output[0].Depth; cou_Index++)
				{
					if (p_Output_Type == 0)
					{
						*p_SF << char(CAN[p_Assembly]->Output[0].get_Pattern_Index(cou_Index));
					}
					if (p_Output_Type == 1)
					{
						*p_SF << CAN[p_Assembly]->Output[0].get_Pattern_Index(cou_Index) << " ";
					}
				}
			}
		}
	}

	void save_Node_To_File(std::ofstream* p_SF, c_Node* p_Node, int p_Output_Type = 0)
	{
		if ((p_SF == NULL) || (p_Node == NULL)) { return; }

		// Check if the flag file exists and can be opened
		if (p_SF->is_open())
		{
			*p_SF << p_Node->NID;
			*p_SF << " ";
			*p_SF << p_Node->RC;
			*p_SF << " ";
			*p_SF << p_Node->Type;
			*p_SF << " ";
			*p_SF << p_Node->State;
			*p_SF << " ";
			*p_SF << p_Node->Dendrite_Count;
			*p_SF << " ";
			for (int cou_D = 0; cou_D < p_Node->Dendrite_Count; cou_D++)
			{
				*p_SF << p_Node->Dendrites[cou_D]->NID;
				*p_SF << " ";
			}

			//Shouldn't need the axons,
			/*
			*p_SF << p_Node->Axon_Hillock_Count;
			*p_SF << " ";
			for (int cou_H = 0; cou_H < p_Node->Axon_Hillock_Count; cou_H++)
			{
				*p_SF << p_Node->Axon_Count[cou_H];
				*p_SF << " ";
				for (int cou_A = 0; cou_A < p_Node->Axon_Count[cou_H]; cou_A++)
				{
					*p_SF << p_Node->Axons[cou_H][cou_A]->NID;
					*p_SF << " ";
				}
			}
			*/
		}
	}

	void save_Assemblies(std::ofstream* p_SF)
	{
		*p_SF << Assembly_Count;

		for (int cou_Ass=0;cou_Ass<Assembly_Count;cou_Ass++)
		{
			*p_SF << "\n";
			*p_SF << Assembly_Names[cou_Ass] << " ";
			*p_SF << CAN_Type[cou_Ass] << " ";
			*p_SF << CAN[cou_Ass]->State_Nodes_Index;
		}
	}

	//Save the node network one node at a time.
	void save_Node_Network(std::ofstream* p_SF)
	{
		*p_SF << "\n";
		*p_SF << Nodes.Node_Count;

		c_Node* tmp_Node;
		tmp_Node = Nodes.Root;

		while (tmp_Node != NULL)
		{
			*p_SF << "\n";

			save_Node_To_File(p_SF, tmp_Node);

			tmp_Node = tmp_Node->Next;
		}
	}

	void save_State_Trees(std::ofstream* p_SF)
	{
		*p_SF << "\n" << Nodes.State_Node_Tree_Count;

		for (int cou_State = 0; cou_State < Nodes.State_Node_Tree_Count; cou_State++)
		{
			Nodes.State_Nodes[cou_State]->save_Tree(p_SF);
		}
	}

	void save(std::string p_FName)
	{
		std::ofstream tmp_Output_File;

		tmp_Output_File.open(p_FName, std::ios::ate);

		if (tmp_Output_File.is_open())
		{
			save_Assemblies(&tmp_Output_File);
			save_Node_Network(&tmp_Output_File);
			save_State_Trees(&tmp_Output_File);
		}

		tmp_Output_File.close();
	}

	void load_Assemblies(std::ifstream* p_SF)
	{
		int tmp_Assembly_Count;
		*p_SF >> tmp_Assembly_Count;

		std::string tmp_Name = "";
		std::string tmp_Type = "";
		int tmp_State_Index = 0;

		for (int cou_Ass = 0; cou_Ass < tmp_Assembly_Count; cou_Ass++)
		{
			*p_SF >> tmp_Name;
			*p_SF >> tmp_Type;
			*p_SF >> tmp_State_Index;

			register_Assembly(tmp_Type, tmp_Name);
			set_State_Nodes_Index(cou_Ass, tmp_State_Index);
		}
	}

	void load_Node(std::ifstream* p_SF)
	{

	}

	void load_Node_Network(std::ifstream* p_SF)
	{
		uint64_t tmp_NID = 0;
		double tmp_RC = 0.0;
		int tmp_Type = 0.0;
		uint64_t tmp_State = 0;
		int tmp_Dendrite_Count = 0;
		int * tmp_Dendrite_NID = NULL;

		c_Node** tmp_Dendrites = NULL;
		c_Node* tmp_Node = NULL;

		uint64_t tmp_Node_Count = 0;
		
		*p_SF >> tmp_Node_Count;

		std::cout << "\n Found " << tmp_Node_Count << " nodes.";

		for (int cou_Node = 0; cou_Node < tmp_Node_Count; cou_Node++)
		{
			std::cout << "\n";
			*p_SF >> tmp_NID;
			std::cout << tmp_NID << " ";

			*p_SF >> tmp_RC;
			std::cout << tmp_RC << " ";
			*p_SF >> tmp_Type;
			std::cout << tmp_Type << " ";
			*p_SF >> tmp_State;
			std::cout << tmp_State << " ";

			//This is because node 0 is already made
			if (tmp_NID != 0)
			{
				tmp_Node = new_Node();
			}


			if (tmp_Node != NULL)
			{
				tmp_Node->set_Type(tmp_Type);
				tmp_Node->RC = tmp_RC;
			}

			*p_SF >> tmp_Dendrite_Count;
			std::cout << tmp_Dendrite_Count << " ";

			tmp_Dendrite_NID = new int[tmp_Dendrite_Count];
			tmp_Dendrites = new c_Node*[tmp_Dendrite_Count];

			for (int cou_D = 0; cou_D < tmp_Dendrite_Count; cou_D++)
			{
				*p_SF >> tmp_Dendrite_NID[cou_D];
				std::cout << tmp_Dendrite_NID[cou_D] << " ";

				tmp_Dendrites[cou_D] = Nodes.get_Node_Ref_By_NID(tmp_Dendrite_NID[cou_D]);
			}

			if (tmp_Dendrite_Count > 0) { create_Connections(tmp_Node, tmp_Dendrite_Count, tmp_Dendrites); }
		}

		if (tmp_Dendrite_NID != NULL) { delete[] tmp_Dendrite_NID; tmp_Dendrite_NID = NULL; }
		if (tmp_Dendrites != NULL) { delete[] tmp_Dendrites; tmp_Dendrites = NULL; }

		//Now gather state nodes.


	}

	void load_State_Trees(std::ifstream* p_SF)
	{
		//This number should exist already in this system from registering the constructs, but we gather it here today to say our...
		int tmp_State_Tree_Count = 0;

		*p_SF >> tmp_State_Tree_Count;
		
		std::cout << "\n Loading [ " << tmp_State_Tree_Count << " ] State Trees...";

		int tmp_State_Node_Count = 0;

		uint64_t tmp_NID = 0;
		uint64_t tmp_State = 0;

		for (int cou_ST = 0; cou_ST < tmp_State_Tree_Count; cou_ST++)
		{
			*p_SF >> tmp_State_Node_Count;

			std::cout << "\n Found [ " << tmp_State_Node_Count << " ] State Bindings...";

			for (int cou_SN = 0; cou_SN < tmp_State_Node_Count; cou_SN++)
			{
				*p_SF >> tmp_NID;
				*p_SF >> tmp_State;

				std::cout << "\n NID " << tmp_NID << " State " << tmp_State;
				
				Nodes.assign_State_Node(cou_ST, Nodes.get_Node_Ref_By_NID(tmp_NID), tmp_State);
			}
		}
	}

	void load(std::string p_FName)
	{
		std::ifstream tmp_Load_File;

		tmp_Load_File.open(p_FName);

		if (tmp_Load_File.is_open())
		{
			load_Assemblies(&tmp_Load_File);
			load_Node_Network(&tmp_Load_File);
			load_State_Trees(&tmp_Load_File);
		}

		tmp_Load_File.close();
	}

	//    ---==  gather_treetop_node [ASSEMBLY_ID]  ==---
	//		This writes the current treetop node of the given assembly to the Assembly_Output_Files[p_Assembly] file. This does not erase the file.
	void gather_Treetop_Node(int p_Assembly)
	{
		std::ofstream tmp_Output_File;
		c_Node* tmp_Treetop = NULL;

		tmp_Treetop = get_Treetop(p_Assembly);

		tmp_Output_File.open(Assembly_Output_Files[p_Assembly], std::ios::app);

		write_Node_To_File(p_Assembly, &tmp_Output_File, tmp_Treetop);

		tmp_Output_File.close();
	}

	//    ---==  gather_treetop_node_uint [ASSEMBLY_ID]  ==---
	//		This writes the current treetop node of the given assembly to the Assembly_Output_Files[p_Assembly] file. This does not erase the file.
	void gather_Treetop_Node_uint(int p_Assembly)
	{
		std::ofstream tmp_Output_File;
		c_Node* tmp_Treetop = NULL;

		tmp_Treetop = get_Treetop(p_Assembly);

		tmp_Output_File.open(Assembly_Output_Files[p_Assembly], std::ios::app);

		write_Node_To_File(p_Assembly, &tmp_Output_File, tmp_Treetop, 1);

		tmp_Output_File.close();
	}

	//    ---==  gather_treetop_NID [ASSEMBLY_ID]  ==---
	//		This writes only the NID of the current treetop to the file. Does not erase the file.
	void gather_Treetop_NID(int p_Assembly)
	{
		std::ofstream tmp_Output_File;
		c_Node* tmp_Treetop = NULL;

		tmp_Treetop = get_Treetop(p_Assembly);

		tmp_Output_File.open(Assembly_Output_Files[p_Assembly], std::ios::app);

		if (tmp_Treetop != NULL)
		{
			tmp_Output_File << tmp_Treetop->NID;
		}
		else
		{
			tmp_Output_File << "NULL";
		}
		tmp_Output_File.close();
	}



	//    ---======================================================================---
	//   ---========================================================================---
	//  ---==   Output the assembly input, output, scaffolds, node network, etc.   ==---
	//   ---========================================================================---
	//    ---======================================================================---

	//    ---==  output_assemblies  ==---
	//		Outputs the assemblies currently registered.
	void output_Assemblies()
	{
		for (int cou_Ass = 0; cou_Ass < Assembly_Count; cou_Ass++)
		{
			std::cout << "\n [" << cou_Ass << "]: " << Assembly_Names[cou_Ass] << " - " << CAN_Type[cou_Ass];
		}
	}

	//      ---==================================---
	//     ---====================================---
	//    ---======================================---
	//   ---========================================---
	//  ---==   Generic commands for the engine.   ==---
	//   ---========================================---
	//    ---======================================---
	//     ---====================================---
	//      ---==================================---



	//    ---==  clear_output  ==---
	int clear_Output(int p_Assembly)
	{
		std::ofstream clsFlagFile(Assembly_Output_Files[p_Assembly], std::ios::ate);

		// Check if the flag file exists and can be opened
		if (clsFlagFile.is_open())
		{
			std::cout << "\n\n --== Resetting " << Assembly_Output_Files[p_Assembly] << " File ==--";
		}

		clsFlagFile.close();
		return 1;
	}


	//    ---==  output_newline  ==---
	int output_Newline(int p_Assembly)
	{
		std::ofstream clsFlagFile(Assembly_Output_Files[p_Assembly], std::ios::app);

		// Check if the flag file exists and can be opened
		if (clsFlagFile.is_open())
		{
			clsFlagFile << "\n";
		}

		clsFlagFile.close();
		return 1;
	}

};

/** @}*/