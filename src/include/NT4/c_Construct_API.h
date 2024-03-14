//It annoys the shit out of me to put the count before the array in the argument list, but the text server is structured as such, mostly because I'm being lazy and don't want to write a complex interpreter atm, so this is structured as such.

//The construct encapsulates the node network, the CANs, I/O, granulation filter, uinterface, actuator interface, and I/O tables.
//The result is that after setting up the network the user can use it like a black box.
class c_Construct_API
{
public:
	
	c_Construct Base;

	c_Construct_API()
	{
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

	//---==  set_type [NID] [TYPE]  ==---//
    //(0: State), (1: Branch), (2: Treetop), (3: State/Treetop)
	void set_Type(uint64_t p_NID, uint8_t p_Type)
	{
		Base.set_Type(Base.get_Node_Ref_By_NID(p_NID), p_Type);
	}


	
	//---==  add_axon_index [NID] [Axon_NID] [INDEX]  ==---//
	//Adds an axon to the axon list at the given index, if the index doesn't exist then exist it with resize_Axon_Hillocks()
	void add_Axon_Index(uint64_t p_NID, uint64_t p_Axon_NID, int p_Index)
	{
		Base.add_Axon_Index(Base.get_Node_Ref_By_NID(p_NID), Base.get_Node_Ref_By_NID(p_Axon_NID), p_Index);
	}


	
	//---==  set_Dendrites [NID] [DENDRITE_IDS[]] [COUNT]  ==---//
    //Sets the dendrites of the node.
    //This assumes the node has no dendrites yet, if it does you be dangling and jangling
	void set_Dendrites(uint64_t p_NID, int p_Count, uint64_t* p_Dendrite_NIDs)
	{
		c_Node** tmp_Dendrites = NULL;
		tmp_Dendrites = new c_Node*[p_Count];

		for (int cou_Index = 0; cou_Index < p_Count; cou_Index++)
		{
			tmp_Dendrites[cou_Index] = Base.get_Node_Ref_By_NID(p_Dendrite_NIDs[cou_Index]);
		}

		Base.set_Dendrites(Base.get_Node_Ref_By_NID(p_NID), p_Count, tmp_Dendrites);

		for (int cou_Index = 0; cou_Index < p_Count; cou_Index++)
		{
			tmp_Dendrites[cou_Index] = NULL;
		}

		if (tmp_Dendrites != NULL) { delete[] tmp_Dendrites; tmp_Dendrites = NULL; }
	}


	//---==  does_Upper_Tier_Connection_Exist NODE_COUNT NODES[]  ==---//
    //Searches the axons to see if an upper tier connection exists.
    //This is always called from the first leg, that is why we separate _F from normal.
	uint64_t does_Upper_Tier_Connection_Exist(int p_Count, uint64_t* p_Nodes)
	{
		c_Node** tmp_Nodes = NULL;
		tmp_Nodes = new c_Node*[p_Count];
		c_Node * return_NID = NULL;

		for (int cou_Index = 0; cou_Index < p_Count; cou_Index++)
		{
			tmp_Nodes[cou_Index] = Base.get_Node_Ref_By_NID(p_Nodes[cou_Index]);
		}

		return_NID = Base.does_Upper_Tier_Connection_Exist(p_Count, tmp_Nodes);

		for (int cou_Index = 0; cou_Index < p_Count; cou_Index++)
		{
			tmp_Nodes[cou_Index] = NULL;
		}

		if (tmp_Nodes != NULL) { delete[] tmp_Nodes; tmp_Nodes = NULL; }

		if (return_NID != NULL) { return return_NID->NID; }
		return 0;
	}


	//---==  does_Lower_Connection_Exist [NID] [NODES[]] [NODE_COUNT]  ==---//
    //Checks if the given node matches a dendrite on the right leg.
	bool does_Lower_Connection_Exist(uint64_t p_NID, int p_Count, uint64_t* p_Nodes)
	{
		c_Node** tmp_Nodes = NULL;
		tmp_Nodes = new c_Node*[p_Count];
		bool return_Result = 0;

		for (int cou_Index = 0; cou_Index < p_Count; cou_Index++)
		{
			tmp_Nodes[cou_Index] = Base.get_Node_Ref_By_NID(p_Nodes[cou_Index]);
		}

		return_Result = Base.does_Lower_Connection_Exist(Base.get_Node_Ref_By_NID(p_NID), p_Count, tmp_Nodes);

		for (int cou_Index = 0; cou_Index < p_Count; cou_Index++)
		{
			tmp_Nodes[cou_Index] = NULL;
		}

		if (tmp_Nodes != NULL) { delete[] tmp_Nodes; tmp_Nodes = NULL; }

		return return_Result;
	}



	//---==  bind_State [NID] [STATE_UINT]  ==---//
    //Binds a node to a quanta of data, the state of the input.
	void bind_State(uint64_t p_NID, uint64_t p_State)
	{
		Base.bind_State(Base.get_Node_Ref_By_NID(p_NID), p_State);
	}



	//---==  bp_O [NID]  ==---//
    //Initiates a backpropagation that outputs the pattern represented by this node.
	void bp_O(uint64_t p_NID)
	{
		Base.bp_O(Base.get_Node_Ref_By_NID(p_NID));
	}



	//---==  output_Node_Raw [NID]  ==---//
	//Outputs the ugly raw info dump for the node.
	void output_Node_Raw(uint64_t p_NID)
	{
		Base.output_Node_Raw(Base.get_Node_Ref_By_NID(p_NID));
	}



	//---==  output_Node_Char [NID]  ==---//
	//Casts the node address to char() and outputs it.
	void output_Node_Char(uint64_t p_NID)
	{
		Base.output_Node_Char(Base.get_Node_Ref_By_NID(p_NID));
	}


	
  
  
//    ---==================---
//   ---====================---
//  ---==   Node_Network   ==---
//   ---====================---
//    ---==================---
  
	//---==  register_New_Construct  ==---//
	int register_New_Construct()
	{
		return Base.register_New_Construct();
	}

  
	//---==  new_Node  ==---//
	//Creates a new node and adds it to the fractal tree.
	//Each node is stored as a link in a linked list.
	uint64_t new_Node()
	{
		return (Base.new_Node())->NID;
	}
  

	//---==  new_State_Node [ASSEMBLY_ID] [STATE_DOUBLE]  ==---//
	//Creates a new node, then adds it to the state tree.
	//Assumes the construct is already registered so the index is valid.
	uint64_t new_State_Node(int p_Assembly, double p_State)
	{
		return (Base.new_State_Node(p_Assembly, p_State))->NID;
	}
  

	//---==  create_Connections [TO_NID] [FROM_NIDS[]] [COUNT]  ==---//
	//Creates a connection between nodes.
	//p_To forms dendritic connections to p_From, and on p_From you have the axonic connections.
	void create_Connections(uint64_t p_To_NID, int p_Count, uint64_t* p_From_NID)
	{
		c_Node** tmp_From = NULL;
		tmp_From = new c_Node * [p_Count];

		for (int cou_Index = 0; cou_Index < p_Count; cou_Index++)
		{
			tmp_From[cou_Index] = Base.get_Node_Ref_By_NID(p_From_NID[cou_Index]);
		}

		Base.create_Connections(Base.get_Node_Ref_By_NID(p_To_NID), p_Count, tmp_From);

		for (int cou_Index = 0; cou_Index < p_Count; cou_Index++)
		{
			tmp_From[cou_Index] = NULL;
		}

		if (tmp_From != NULL) { delete[] tmp_From; tmp_From = NULL; }
	}
  

	//---==  does_Upper_Tier_Connection_Exist [LEGS] [LEG_COUNT]  ==---//
	//Checks if an upper tier node exists.
	uint64_t does_Upper_Tier_Connection_Exist_Network(int p_Count, uint64_t* p_Legs_NID)
	{
		c_Node** tmp_Legs = NULL;
		tmp_Legs = new c_Node * [p_Count];
		c_Node* tmp_Return_Node = NULL;

		for (int cou_Index = 0; cou_Index < p_Count; cou_Index++)
		{
			tmp_Legs[cou_Index] = Base.get_Node_Ref_By_NID(p_Legs_NID[cou_Index]);
		}

		tmp_Return_Node = Base.does_Upper_Tier_Connection_Exist(p_Count, tmp_Legs);

		for (int cou_Index = 0; cou_Index < p_Count; cou_Index++)
		{
			tmp_Legs[cou_Index] = NULL;
		}

		if (tmp_Legs != NULL) { delete[] tmp_Legs; tmp_Legs = NULL; }

		if (tmp_Return_Node != NULL) { return tmp_Return_Node->NID; }
		return NULL;
	}
  

	//---==  get_Upper_Tier_Node [LEGS] [LEG_COUNT]  ==---//
	//Gets an upper tier node based on the given legs. Will create it if not found.
	uint64_t get_Upper_Tier_Node(int p_Count, uint64_t* p_Legs_NID)
	{
		c_Node** tmp_Legs = NULL;
		tmp_Legs = new c_Node * [p_Count];
		c_Node* tmp_Return_Node = NULL;

		for (int cou_Index = 0; cou_Index < p_Count; cou_Index++)
		{
			tmp_Legs[cou_Index] = Base.get_Node_Ref_By_NID(p_Legs_NID[cou_Index]);
		}

		tmp_Return_Node = Base.get_Upper_Tier_Node(p_Count, tmp_Legs);

		for (int cou_Index = 0; cou_Index < p_Count; cou_Index++)
		{
			tmp_Legs[cou_Index] = NULL;
		}

		if (tmp_Legs != NULL) { delete[] tmp_Legs; tmp_Legs = NULL; }

		if (tmp_Return_Node != NULL) { return tmp_Return_Node->NID; }
		return NULL;
	}

  
	//---==  does_State_Node_Exist [ASSEMBLY_ID] [STATE]  ==---//
	//If a state node exists in the given construct index then return it.
	//Otherwise return NULL.
	//This assumes the [Index] is valid
	uint64_t does_State_Node_Exist(int p_Assembly, uint64_t p_State)
	{
		c_Node* tmp_Node = NULL;
		tmp_Node = Base.does_State_Node_Exist(p_Assembly, p_State);

		if (tmp_Node != NULL) { return tmp_Node->NID; }
		return 0;
	}
  

	//---==  get_State_Node [ASSEMBLY_ID] [STATE]  ==---//
	//Checks to see if a node in the given assembly is bound to the given state, if not the node is created.
	uint64_t get_State_Node(int p_Assembly, uint64_t p_State)
	{
		c_Node* tmp_Node = NULL;
		tmp_Node = Base.get_State_Node(p_Assembly, p_State);

		if (tmp_Node != NULL) { return tmp_Node->NID; }
		return 0;
	}
  

	//---==  output_BP  ==---//
	//Iterates through every node and outputs their bp_O()
	void output_BP()
	{
		Base.output_BP();
	}

  
	//---==  output_BP_NID [NID]  ==---//
	//Finds given NID and outputs the bp_O()
	void output_BP_NID(int p_NID)
	{
		Base.output_BP_NID(p_NID);
	}

  
	//---==  output_Node_Network  ==---//
	//Outputs all of the nodes as raw.
	void output_Node_Network()
	{
		Base.output_Node_Network();
	}



	//    ---=========---
	//   ---===========---
	//  ---==   CAN   ==---
	//   ---===========---
	//    ---=========---

	//---==  encode [ASSEMBLY_ID]  ==---//
	//This encodes the p_Input data, if the nodes aren't found they are created, used for training.
	//Encodes the current input of the given assembly. How it does this is determined by what type of assembly it is.
	void encode(int p_Assembly)
	{
		Base.encode(p_Assembly);
	}


	//    ---==  query [ASSEMBLY_ID]  ==---
	//This sets up the scaffold as encode does, but it doesn't create nodes if they aren't found, they remain NULL in the scaffold, this we call a NULLCAN
	//Used for querying the network, you input, fill the NULLCAN, charge the network, then gather the outputs.
	//Suggested for use before encoding (if using learning mode and not locked_to_initial_training_mode) otherwise it will also find the current trace as the perfect match.
	//		Queries the network with the current input set for the given assembly.
		//Passes the values to an assembly to encode.
	void query(int p_Assembly)
	{
		Base.query(p_Assembly);
	}

	//    ---==  query_spacial [ASSEMBLY_ID]  ==---
	//		Queries the network with the current input set of the given assembly, but the input index determines what leg is charged. Meaning if an input at index [3] is charged then only upper tier nodes connected on axon hillock [3] will be charged.
		//Passes the values to an assembly to encode.
	void query_Spacial(int p_Assembly)
	{
		Base.query(p_Assembly, NULL, 0, 1);
	}

	//    ---==  query_given_index [ASSEMBLY_ID] [INDEX]  ==---
	//		Queries the network with the current input set, however, every input node is charged on the given index. If INDEX is [3] then all nodes in the input set will be charging using axon hillock [3].
		//Passes the values to an assembly to encode.
	void query_Given_Index(int p_Assembly, int p_Index)
	{
		Base.query(p_Assembly, NULL, 0, 2, p_Index);
	}

	//    ---==  query_given_legs[ASSEMBLY_ID] [LEG_COUNT] [LEG_0] [LEG_1] [...]  ==---
	void query_Given_Legs(int p_Assembly, int p_Leg_Count, int* p_Legs)
	{
		Base.query(p_Assembly, NULL, 0, 3, p_Leg_Count, p_Legs);
	}


	//---==  submit_Set [ASSEMBLY_ID] [INPUT_UINT[]] [DEPTH]  ==---//
	//This allows for passing unordered sets of nodes
	void submit_Set(int p_Assembly, int p_Depth, uint64_t* p_Input)
	{
		Base.submit_Set(p_Assembly, p_Depth, p_Input);
	}


	//---==  get_Treetop [ASSEMBLY_ID]  ==---//
	//Gets the treetop node for a given assembly.
	//This returns the treetop node at a given index, for most structures this will be a single node, but for those like stiched-base networks with a treetop node count equal to the input node count then you can access them by index.
	//Gets the treetop node for a given assembly.
	uint64_t get_Treetop_NID(int p_Assembly)
	{
		return Base.get_Treetop_NID(p_Assembly);
	}


	//---==  gather_Given_Trace [ASSEMBLY_ID] [NID]  ==---//
	//Gets a single trace from a given node. Puts it into the output.
	void gather_Given_Trace(int p_Assembly, uint64_t p_NID)
	{
		Base.gather_Given_Trace(p_Assembly, p_NID);
	}


	//---==  gather_All_Traces [ASSEMBLY]  ==---//
	//Gathers all the traces as it says.
	void gather_All_Traces(int p_Assembly)
	{
		Base.gather_All_Traces(p_Assembly);
	}


	//---==  reset_Input [ASSEMBLY_ID]  ==---//
	//Wipe the input array.
	void reset_Input(int p_Assembly)
	{
		Base.reset_Input(p_Assembly);
	}


	//---==  set_State_Nodes_Index [ASSEMBLY_ID] [INDEX]  ==---//
	//Sets the index for the state_Node_Tree in the c_Node_Network::State_Nodes[]
	void set_State_Nodes_Index(int p_Assembly, int p_Index)
	{
		Base.set_State_Nodes_Index(p_Assembly, p_Index);
	}


	

	//---==  output_Scaffold [ASSEMBLY_ID]  ==---//
	//Outputs the scaffold as addresses.
	void output_Scaffold(int p_Assembly)
	{
		Base.output_Scaffold(p_Assembly);
	}


	//---==  output_Input [ASSEMBLY_ID]  ==---//
	//		Outputs the input of the given assembly to the console.
	void output_Input(int p_Assembly)
	{
		Base.output_Input(p_Assembly);
	}


	//---==  output_Input [ASSEMBLY_ID]  ==---//
	//		Outputs the input of the given assembly to the console.
	void output_Input_uint(int p_Assembly)
	{
		Base.output_Input_uint(p_Assembly);
	}


	//---==  output_Output [ASSEMBLY_ID]  ==---//
	//The output trace set is output.
	void output_Output(int p_Assembly)
	{
		Base.output_Output(p_Assembly);
	}


	//---==  output_Output [ASSEMBLY_ID]  ==---//
	//The output trace set is output.
	void output_Output_uint(int p_Assembly)
	{
		Base.output_Output_uint(p_Assembly);
	}


	//---==  output_Scaffold_Char [ASSEMBLY_ID]  ==---//
	//Each address is typecast to a char to give a pseudo-unique look to each node. For monke brain.
	void output_Scaffold_Char(int p_Assembly)
	{
		Base.output_Scaffold_Char(p_Assembly);
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
		return Base.register_Assembly(p_Type, p_Assembly_Name);
	}




	//    ---=====================---
	//   ---=======================---
	//  ---==   Input handling.   ==---
	//   ---=======================---
	//    ---=====================---

//    ---==  load_input  ==---
	int load_Input(int p_Assembly)
	{
		return Base.load_Input(p_Assembly);
	}

	int load_Input_uint(int p_Assembly)
	{
		return Base.load_Input_uint(p_Assembly);
	}

	//    ---==  set_input [ASSEMBLY_ID] [INPUT_STRING]  ==---
		//Set the value to the passed 1D string of uint64_t
	void set_Input(int p_Assembly, std::string p_Input)
	{
		Base.set_Input(p_Assembly, p_Input);
	}

	//    ---==  set_input_uint [ASSEMBLY_ID] [ARRAY_DEPTH] [UINT_ARRAY]  ==---
		//Set the value to the passed 1D string of uint64_t
	void set_Input_uint(int p_Assembly, int p_Depth, uint64_t* p_Input)
	{
		Base.set_Input_uint(p_Assembly, p_Depth, p_Input);
	}




	//---==  set_Input [ASSEMBLY_ID] [INPUT_UINT[]] [INPUT_DEPTH]  ==---//
	//Sets the input to the given uint64_t array.
	//The input array is 1D
	void set_Input(int p_Assembly, int p_Input_Depth, uint64_t* p_Input)
	{
		Base.set_Input(p_Assembly, p_Input_Depth, p_Input);
	}



	//Sets the input to the given uint64_t 2d array.
	//The input array is 2D
	void set_2D_Input_uint(int p_Assembly, int p_X_Depth, int p_Y_Depth, uint64_t** p_Input)
	{
		Base.set_2D_Input(p_Assembly, p_X_Depth, p_Y_Depth, p_Input);
	}


	//Sets the input to the given uint64_t 3d array.
	//The input array is 3D
	void set_3D_Input_uint(int p_Assembly, int p_X_Depth, int p_Y_Depth, int p_Z_Depth, uint64_t*** p_Input)
	{
		Base.set_3D_Input(p_Assembly, p_X_Depth, p_Y_Depth, p_Z_Depth, p_Input);
	}



	//---==  set_Input_String [ASSEMBLY_ID] [INPUT_STR]  ==---//
	//This is used for setting the input array to reflect a sequence of characters.
	void set_Input_String(int p_Assembly, std::string p_Input)
	{
		Base.set_Input_String(p_Assembly, p_Input);
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
		Base.gather_Given_Node(p_Assembly, p_NID);
	}

	//		Writes the given node's data down in the Assembly_Output_Files[p_Assembly] file.
	void gather_Given_Node_uint(int p_Assembly, uint64_t p_NID)
	{
		Base.gather_Given_Node_uint(p_Assembly, p_NID);
	}

	//    ---==  gather_all_nodes [ASSEMBLY_ID]  ==---
	//		This writes the entire network to the Assembly_Output_Files[p_Assembly] file. Note, the output patterns are treated as character.
		//It uses the passed assembly to output the nodes by putting it into the output of that assembly, then into the file.
	void gather_All_Nodes(int p_Assembly)
	{
		Base.gather_All_Nodes(p_Assembly);
	}

	//    ---==  gather_all_nodes_uint [ASSEMBLY_ID]  ==---
	//		This writes the entire network to the Assembly_Output_Files[p_Assembly] file. Note, the output patterns are treated as uint.
	void gather_All_Nodes_uint(int p_Assembly)
	{
		Base.gather_All_Nodes_uint(p_Assembly);
	}


	//    ---==================================================================---
	//   ---====================================================================---
	//  ---==   The output of a given assembly is read into the output file.   ==---
	//   ---====================================================================---
	//    ---==================================================================---


	//    ---==  gather_output [ASSEMBLY_ID]  ==---
	//		Takes every trace in the given Assemblies output trace array and writes them to the Assembly_Output_Files[p_Assembly] file, note the output state patterns are treated as char.
	void gather_Output(int p_Assembly)
	{
		Base.gather_Output(p_Assembly);
	}

	//    ---==  gather_output_uint [ASSEMBLY_ID]  ==---
	//		Takes every trace in the given Assemblies output trace array and writes them to the Assembly_Output_Files[p_Assembly] file, note the output state patterns are treated as uint.
	void gather_Output_uint(int p_Assembly)
	{
		Base.gather_Output_uint(p_Assembly);
	}


	//    ---=============================---
	//   ---===============================---
	//  ---==   Treetop node gathering.   ==---
	//   ---===============================---
	//    ---=============================---

	//    ---==  gather_treetop_node [ASSEMBLY_ID]  ==---
	//		This writes the current treetop node of the given assembly to the Assembly_Output_Files[p_Assembly] file. This does not erase the file.
	void gather_Treetop_Node(int p_Assembly)
	{
		Base.gather_Treetop_Node(p_Assembly);
	}

	//    ---==  gather_treetop_node_uint [ASSEMBLY_ID]  ==---
	//		This writes the current treetop node of the given assembly to the Assembly_Output_Files[p_Assembly] file. This does not erase the file.
	void gather_Treetop_Node_uint(int p_Assembly)
	{
		Base.gather_Treetop_Node_uint(p_Assembly);
	}

	//    ---==  gather_treetop_NID [ASSEMBLY_ID]  ==---
	//		This writes only the NID of the current treetop to the file. Does not erase the file.
	void gather_Treetop_NID(int p_Assembly)
	{
		Base.gather_Treetop_NID(p_Assembly);
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
		Base.output_Assemblies();
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
		return Base.clear_Output(p_Assembly);
	}


	//    ---==  output_newline  ==---
	int output_Newline(int p_Assembly)
	{
		return Base.output_Newline(p_Assembly);
	}


	void save(std::string p_FName)
	{
		Base.save(p_FName);
	}

	void load(std::string p_FName)
	{
		Base.load(p_FName);
	}
};