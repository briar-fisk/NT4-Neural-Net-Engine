
//This functions to create a trace where all legs are connected to one node.
//Load up the input, hit encode(), boom you single permutation of all legs bound to that node.
//Most useful when each index in the input is a separate state space from the other indexes, sensors == good, language == shared states across words !good
class c_CAN_Many_To_One : public c_Base_CAN
{
public:

	//Each CAN node is just a pointer to a node in the network.
	//For this CAN the scaffold is 2 tiers, the bottom being the state tier, and then one node on tier[1] for the treetop/symbol node.
	//So for this CAN we don't track the number of tiers, that is always 2, and the number of nodes is equal to the input depth for the state(quanta) tier, and 1 for the treetop.
	c_Node** Scaffold[2];
	int State_Depth; //We track this so that if the input is changed we can still properly delete the scaffold.

	//The temporary buffer used during charging.
	c_Charging_Buffer tmp_Buffman;

	c_CAN_Many_To_One()
	{
		NNet = NULL;

		Input.reset();
		Output = NULL;
		Output_Depth = 0;

		Scaffold[0] = NULL;
		State_Depth = 0;
		Scaffold[1] = new c_Node * [1];

		State_Nodes_Index = 0;
	}

	~c_CAN_Many_To_One()
	{
		NNet = NULL;
		reset_Scaffold();
		reset_Input();
	}

	//Resets the CAN to NULL, and deletes the state tier + treetop.
	void reset_Scaffold()
	{
		if (Scaffold[0] != NULL)
		{
			//Make sure to NULL the scaffold first.
			//DO NOT DELETE THEM, the addresses are for the node network, deleting them here will cause null pointer deletion in Node_Network.
			for (int cou_Index = 0; cou_Index < State_Depth; cou_Index++)
			{
				Scaffold[0][cou_Index] = NULL;
			}
			delete[] Scaffold[0];
			Scaffold[0] = NULL;
		}
		Scaffold[1][0] = NULL;
	}

	//This sets up the actual CAN scaffold to use.
	//It assumes the input array is filled out, the size is based upon that array.
	void setup_CAN_Scaffold()
	{
		//Take it out back and put it down.
		reset_Scaffold();

		//Sizing her up!
		State_Depth = Input.Depth;

		//Setup the lowest tier of the scaffold so that every index in the input has a correlated state tier node position on the scaffold.
		Scaffold[0] = new c_Node * [State_Depth];

		//Initialize the scaffold.
		for (int cou_Index = 0; cou_Index < State_Depth; cou_Index++)
		{
			Scaffold[0][cou_Index] = NULL;
		}
		Scaffold[1][0] = NULL;
	}

	//This fills the state tier by querying the node network.
	//p_How: "Encode" == Create the nodes if they aren't found, "Query" == Returns NULL if they aren't found, used for checking if something has been encoded without modifying the network.
	void fill_State(std::string p_How = "Encode")
	{
		std::cout << "\n Encoding with State_Nodes_Index: " << State_Nodes_Index;
		if (p_How == "Encode")
		{
			for (int cou_Index = 0; cou_Index < State_Depth; cou_Index++)
			{
				//Request the state node form the node network using get_State_Node so one is created if not found.
				//We have to make sure we request the correct state tree.
				Scaffold[0][cou_Index] = NNet->get_State_Node(State_Nodes_Index, Input.Data[cou_Index]);
				Scaffold[0][cou_Index]->RC++;

				//If the node is also a treetop then set it to state/treetop.
				if ((Scaffold[0][cou_Index]->Type == 2))
				{
					Scaffold[0][cou_Index]->Type = 3;
				}
			}
		}
		if (p_How == "Query")
		{
			for (int cou_Index = 0; cou_Index < State_Depth; cou_Index++)
			{
				//Request the state node form the node network using get_State_Node so one is created if not found.
				//We have to make sure we request the correct state tree.
				Scaffold[0][cou_Index] = NNet->does_State_Node_Exist(State_Nodes_Index, Input.Data[cou_Index]);
			}
		}
	}

	//Just one node at the tippy top.
	//p_How: "Encode" == Create the nodes if they aren't found, "Query" == Returns NULL if they aren't found, used for checking if something has been encoded without modifying the network.
	void fill_Scaffold(std::string p_How = "Encode")
	{
		if (State_Depth == 0)
		{
			Scaffold[1][0] = NULL;

			return;
		}
		if (p_How == "Encode")
		{
			//We request a node that links the entire state tier together.
			Scaffold[1][0] = NNet->get_Upper_Tier_Node(Scaffold[0], State_Depth, 2);
			Scaffold[1][0]->RC++;
		}
		if (p_How == "Query")
		{
			//We request a node that links the entire state tier together, but do not create them.
			Scaffold[1][0] = NNet->does_Upper_Tier_Connection_Exist(Scaffold[0], State_Depth);
		}
	}

	//Encodes a single trace, forcibly.
	void encode(uint64_t* p_Input = NULL, int p_Depth = 0)
	{
		std::cout << "\n\n<<-- Begin Encoding -->>\n\n";

		//Firstly we gather the inputly
		if ((p_Input != NULL) && (p_Depth > 0)) { set_Input(p_Input, p_Depth); }

		//Set up the scaffold for the nodes to reside in as we build the trace.
		setup_CAN_Scaffold();

		//Work across the state tier to fill it out by requesting state nodes from the NNet, if not found they are created.
		fill_State("Encode");

		//Fills the scaffold out by requesting nodes from the NNet and creating them if they aren't found.
		fill_Scaffold("Encode");

		//To be removed later after testing.
		//output_Input();
		output_Scaffold();
		output_Scaffold_Char();

		std::cout << "\n\n-- End Encoding --\n\n";
	}

	//Style determines whether it charges with normal submission of raw, or if it does the specific leg charging for Chrono.
	void charge_Buffers(int p_Style = -1, int p_Leg = 0, int * p_Legs = NULL)
	{

		tmp_Buffman.reset();

		tmp_Buffman.Input_Position = 0;

		tmp_Buffman.charge_Outputs();

		for (int cou_Input = 0; cou_Input < Input.Depth; cou_Input++)
		{
			if (Scaffold[0][cou_Input] != NULL)
			{
				std::cout << "\n\n ~~++==++~~ Charging Node: CAN[0][" << cou_Input << "]: " << Scaffold[0][cou_Input] << " ~ " << Scaffold[0][cou_Input]->NID;

				//tmp_Buffman.submit(Scaffold[0][cou_Input], (1.0));
				if (p_Style == -1)
				{
					tmp_Buffman.submit(Scaffold[0][cou_Input], (10.0));
				}
				if (p_Style == 1)
				{
					tmp_Buffman.charge_Given_Leg(Scaffold[0][cou_Input], (10.0), cou_Input);
				}
				if (p_Style == 2)
				{
					//p_Leg specifies which leg to charge in this function, p_Legs[] being unused.
					tmp_Buffman.charge_Given_Leg(Scaffold[0][cou_Input], (10.0), p_Leg);
				}
				if (p_Style == 3)
				{
					//p_Leg is used here as the count of elements in p_Legs[].
					tmp_Buffman.charge_Given_Legs(Scaffold[0][cou_Input], p_Leg, p_Legs, (10.0));
				}
			}
		}

		//std::cout << "\n\n++++++++++++++++++++++++++++++ Before ++++++++++++++++++++++++++++++ \n\n";

		//tmp_Buffman.output_All_Buffers();

		tmp_Buffman.gather();

		//std::cout << "\n\n++++++++++++++++++++++++++++++ After ++++++++++++++++++++++++++++++ \n\n";

		//tmp_Buffman.output_All_Buffers();

		while (tmp_Buffman.flg_Not_Done)
		{
			std::cout << "\n\n flg_Not_Done\n";

			tmp_Buffman.charge_Outputs();

			tmp_Buffman.gather();

			//tmp_Buffman.output_All_Buffers();
		}

		std::cout << "\n\n Completed:\n";

		//tmp_Buffman.output_All_Buffers();

		std::cout << "\n\n Treetops:\n";
		tmp_Buffman.output_Treetops();

		c_Charging_Linked_List * tmp_Current_LL = NULL;
		tmp_Current_LL = tmp_Buffman.Treetops.Root;

		/*
		while (tmp_Current_LL != NULL)
		{
			tmp_Current_LL->NID->bp_O();

			tmp_Current_LL = tmp_Current_LL->Next;
		}
		*/
	}

	void gather_Treetops()
	{
		std::cout << "\n\n Gathering Treetops...";

		c_Charging_Linked_List* tmp_Current_LL = NULL;
		tmp_Current_LL = tmp_Buffman.Treetops.Root;

		if (Output != NULL) { delete[] Output; Output = NULL; }

		Output = new c_Trace[tmp_Buffman.Treetops.Depth];
		Output_Depth = tmp_Buffman.Treetops.Depth;

		int tmp_Current_Index = 0;

		c_Linked_List_Handler tmp_Pattern;

		c_Linked_List* tmp_LL_Pat = NULL;

		while (tmp_Current_LL != NULL)
		{
			tmp_Pattern.reset();

			//Get the pattern into a linked list
			tmp_Current_LL->NID->bp_Trace_O(&tmp_Pattern);

			std::cout << "\n NID: " << tmp_Current_LL->NID->NID;
			std::cout << "\n tmp_Pattern.Depth: " << tmp_Pattern.Depth;
			tmp_Pattern.output();

			std::cout << "\n tmp_Pattern.Depth: " << tmp_Pattern.Depth;
			//Copy the pattern over
			Output[tmp_Current_Index].set_Depth(tmp_Pattern.Depth);

			std::cout << "\n Output[" << tmp_Current_Index << "].Depth: " << Output[tmp_Current_Index].Depth;

			std::cout << "\n tmp_Pattern.Depth: " << tmp_Pattern.Depth;

			tmp_LL_Pat = tmp_Pattern.Root;

			//We can iterate through given we know how big the linked list is.
			for (int cou_Index = 0; cou_Index < tmp_Pattern.Depth; cou_Index++)
			{
				Output[tmp_Current_Index].set_Pattern_Index(tmp_LL_Pat->Quanta, cou_Index);
				tmp_LL_Pat = tmp_LL_Pat->Next;
			}

			std::cout << "\n tmp_Current_LL->NID->Current_Charge: " << tmp_Current_LL->NID->Current_Charge;
			std::cout << "\n tmp_Current_LL->Charge: " << tmp_Current_LL->Charge;
			std::cout << "\n tmp_Current_LL->NID->RC: " << tmp_Current_LL->NID->RC;
			Output[tmp_Current_Index].set_Charge(tmp_Current_LL->Charge);
			//Output[tmp_Current_Index].set_Charge(tmp_Current_LL->NID->Current_Charge);
			Output[tmp_Current_Index].set_RC(tmp_Current_LL->NID->RC);
			Output[tmp_Current_Index].set_Treetop(tmp_Current_LL->NID);

			tmp_Current_LL = tmp_Current_LL->Next;
			
			Output[tmp_Current_Index].output(0);
			Output[tmp_Current_Index].output(1);

			tmp_Current_Index++;
		}
	}

	//Gets a single trace from a given node. Puts it into the output.
	void gather_All_Traces()
	{
		std::cout << "\n\n Gathering All Traces!!!";



		c_Node* tmp_Node = NULL;
		tmp_Node = NNet->Root;

		if (Output != NULL) { delete[] Output; Output = NULL; }

		Output = new c_Trace[NNet->Node_Count];
		Output_Depth = NNet->Node_Count;

		int tmp_Current_Index = 0;

		c_Linked_List_Handler tmp_Pattern;

		c_Linked_List* tmp_LL_Pat = NULL;

		while (tmp_Node != NULL)
		{
			std::cout << "\nNode (" << tmp_Node->NID << ") ";
			tmp_Node->bp_O();


			tmp_Pattern.reset();

			//Get the pattern into a linked list
			tmp_Node->bp_Trace_O(&tmp_Pattern);

			std::cout << "\n NID: " << tmp_Node->NID;
			std::cout << "\n tmp_Pattern.Depth: " << tmp_Pattern.Depth;
			tmp_Pattern.output();

			std::cout << "\n tmp_Pattern.Depth: " << tmp_Pattern.Depth;
			//Copy the pattern over
			Output[tmp_Current_Index].set_Depth(tmp_Pattern.Depth);

			std::cout << "\n Output[" << tmp_Current_Index << "].Depth: " << Output[tmp_Current_Index].Depth;

			std::cout << "\n tmp_Pattern.Depth: " << tmp_Pattern.Depth;

			tmp_LL_Pat = tmp_Pattern.Root;

			//We can iterate through given we know how big the linked list is.
			for (int cou_Index = 0; cou_Index < tmp_Pattern.Depth; cou_Index++)
			{
				Output[tmp_Current_Index].set_Pattern_Index(tmp_LL_Pat->Quanta, cou_Index);
				tmp_LL_Pat = tmp_LL_Pat->Next;
			}

			std::cout << "\n tmp_Current_LL->NID->Current_Charge: " << tmp_Node->Current_Charge;
			std::cout << "\n tmp_Current_LL->NID->RC: " << tmp_Node->RC;
			Output[tmp_Current_Index].set_Charge(tmp_Node->Current_Charge);
			//Output[tmp_Current_Index].set_Charge(tmp_Current_LL->NID->Current_Charge);
			Output[tmp_Current_Index].set_RC(tmp_Node->RC);
			Output[tmp_Current_Index].set_Treetop(tmp_Node);

			Output[tmp_Current_Index].output(0);
			Output[tmp_Current_Index].output(1);

			tmp_Current_Index++;

			tmp_Node = tmp_Node->Next;
		}
	}

	//Gets a single trace from a given node. Puts it into the output.
	void gather_Given_Trace(uint64_t p_NID)
	{
		std::cout << "\n\n Gathering Given Trace " << p_NID << "...";

		c_Node* tmp_Node = NULL;
		tmp_Node = NNet->get_Node_Ref_By_NID(p_NID);

		if (Output != NULL) { delete[] Output; Output = NULL; }

		Output = new c_Trace[1];
		Output_Depth = 1;

		int tmp_Current_Index = 0;

		c_Linked_List_Handler tmp_Pattern;

		c_Linked_List* tmp_LL_Pat = NULL;

		tmp_Pattern.reset();

		//Get the pattern into a linked list
		tmp_Node->bp_Trace_O(&tmp_Pattern);

		std::cout << "\n NID: " << tmp_Node->NID;
		std::cout << "\n tmp_Pattern.Depth: " << tmp_Pattern.Depth;
		tmp_Pattern.output();

		//Copy the pattern over
		Output[tmp_Current_Index].set_Depth(tmp_Pattern.Depth);

		std::cout << "\n Output[" << tmp_Current_Index << "].Depth: " << Output[tmp_Current_Index].Depth;

		std::cout << "\n tmp_Pattern.Depth: " << tmp_Pattern.Depth;

		tmp_LL_Pat = tmp_Pattern.Root;

		//We can iterate through given we know how big the linked list is.
		for (int cou_Index = 0; cou_Index < tmp_Pattern.Depth; cou_Index++)
		{
			Output[tmp_Current_Index].set_Pattern_Index(tmp_LL_Pat->Quanta, cou_Index);
			tmp_LL_Pat = tmp_LL_Pat->Next;
		}

		std::cout << "\n tmp_Node->Current_Charge: " << tmp_Node->Current_Charge;
		std::cout << "\n tmp_Node->RC: " << tmp_Node->RC;
		Output[tmp_Current_Index].set_Charge(tmp_Node->Current_Charge);
		//Output[tmp_Current_Index].set_Charge(tmp_Current_LL->NID->Current_Charge);
		Output[tmp_Current_Index].set_RC(tmp_Node->RC);
		Output[tmp_Current_Index].set_Treetop(tmp_Node);

		Output[tmp_Current_Index].output(0);
		Output[tmp_Current_Index].output(1);

		tmp_Current_Index++;
	}


	//Fills out the NULLCAN, does not forcibly encode, and then charges the network.
	//Charging style determines leg charging. May move it to be a setting.
	//     -1: Charge by giving the input node the base charge, normal node charging.
	//      1: Use leg specific charging to charge every node based on its position in the input array. Node[1] would charge axons on Axon[1][n].
	//      2: Charge every input on the given p_Leg, used mainly when inputing single values to charge. Allows you to input Node[x] as the only input to the Chrono and charge it using Axon[4][n], or any axon hillock you choose.
	//		 This allows you to search forward by setting a node early in the time series and searching forwards, or setting it late in the time series and searching backwards.
	void query(uint64_t* p_Input = NULL, int p_Depth = 0, int p_Charging_Style = -1, int p_Leg = 0, int * p_Legs = NULL)
	{
		//Firstly we gather the inputly
		if ((p_Input != NULL) && (p_Depth > 0)) { set_Input(p_Input, p_Depth); }

		//Set up the scaffold for the nodes to reside in as we build the trace.
		setup_CAN_Scaffold();

		//Work across the state tier to fill it out by requesting state nodes from the NNet, if not found they are created.
		fill_State("Query");

		//Fills the scaffold out by requesting nodes from the NNet and creating them if they aren't found.
		fill_Scaffold("Query");

		charge_Buffers(p_Charging_Style, p_Leg, p_Legs);

		gather_Treetops();


		//To be removed later after testing.
		output_Input();
		output_Scaffold();
		output_Scaffold_Char();
		output_Output();
	}

	//This allows for passing unordered sets of nodes
	void submit_Set(uint64_t* p_Input, int p_Depth)
	{
		//Firstly we gather the inputly
		set_Input(p_Input, p_Depth);

		//We only do the query on the state tier. This is because the nodes will not be in a coherent relationship to each other, only their presence is important.
		//The node adress being read in as the state means that the unordered set can be read in to the state tier because the shared memory space precludes duplicates, you just ignore any higher tiers than 0.
		//Work across the state tier to fill it out by requesting state nodes from the NNet, if not found they are created.
		fill_State("Query");

		//Then charge the buffers as normal.
		charge_Buffers();
	}

	//Gets the current treetop at the given index.
	c_Node* get_Treetop(int p_Index = -1)
	{
		//For this network the treetop is the scaffold[1][0] since all lower nodes link to this higher tier node.
		return Scaffold[1][0];
	}

	//Outputs the scaffold.
	void output_Scaffold()
	{
		std::cout << "\n  --==   CAN_Scaffold   ==--";
		std::cout << "\n <- Tier[0] ->";
		for (int cou_Index = 0; cou_Index < State_Depth; cou_Index++)
		{
			std::cout << "\n   [" << cou_Index << "] ";
			if (Scaffold[0][cou_Index] != NULL) { std::cout << " <> " << Scaffold[0][cou_Index]->NID; } else { std::cout << "NULL"; }
		}
		std::cout << "\n <- Tier[1] ->";
		std::cout << "\n   [0] ";
		if (Scaffold[1][0] != NULL) { std::cout << " <> " << Scaffold[1][0]->NID; } else { std::cout << "NULL"; }
	}


	//Outputs the scaffold as character representing the address.
	void output_Scaffold_Char()
	{
		std::cout << "\n\n(" << this << ")\n";
		for (int cou_Index = 0; cou_Index < State_Depth; cou_Index++)
		{
			std::cout << char(Scaffold[0][cou_Index]);
		}
		std::cout << "\n" << char(Scaffold[1][0]);
	}
};
