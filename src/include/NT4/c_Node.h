//The state is stored as uint64_t so you will need to typecast to appropriate datatype.

//Quick n dirty to output the double for the node states
union u_Tmp
{
    double D;
    uint64_t U;
};

class c_Node
{
public:

    //The Node ID, used because the raw address may change upon saving/loading and external hooks need a solid grasp.
    uint64_t NID;

    //The next node in the network.
    c_Node* Next;

    //Axons and the counters. Axons[0][] are the ones connected to the first leg, used in finding UTN
    //Axons are organized by the dendritic index they are connected to, if they connect to leg [0] then they are an axon on hillock [0].
    c_Node*** Axons;
    int* Axon_Count;
    int Axon_Hillock_Count;

    //Dendrites, lower connections, leg order is of highest importance.
    c_Node** Dendrites;
    int Dendrite_Count;

    //The reinforcement counter.
    //A filter function will be built into the c_Neural_Network that applies:
    // exponential growth to reduce new trace impact, one offs and flukes get filtered
    // Sigmoid curve for the top end to prevent extreme bias in highly reinforced nodes.
    double RC;

    //The current charge of the node.
    double Current_Charge;

    //The quanta bound to the node.
    uint64_t State;

    //This tracks the node type.
    //(0: NULL), (1: State), (2: Branch), (3: Treetop), (4: State Treetop)
    short int Type;

    c_Node()
    {
        NID = 0;

        //The next node.
        Next = NULL;

        //Axons and their counters.

        Axons = NULL;
        Axon_Count = 0;
        Axon_Hillock_Count = 0;

        //Dendrites.
        Dendrites = NULL;
        Dendrite_Count = 0;

        //Reinforcement Counters.
        RC = 1.0;

        //The nodes state.
        State = 0;

        //No charge atm.
        Current_Charge = 0.0;

        //The type is 0 to start with.
        Type = 0;
    }

    //(0: State), (1: Branch), (2: Treetop), (3: State/Treetop)
    void set_Type(int p_Type)
    {
        Type = p_Type;
    }

    void resize_Axon_Hillocks(int p_Count)
    {
        c_Node*** tmp_Axons;
        int* tmp_Axon_Count;
        tmp_Axons = new c_Node ** [Axon_Hillock_Count];
        tmp_Axon_Count = new int [Axon_Hillock_Count];

        for (int cou_A = 0; cou_A < Axon_Hillock_Count; cou_A++)
        {
            tmp_Axons[cou_A] = Axons[cou_A];
            Axons[cou_A] = NULL;

            tmp_Axon_Count[cou_A] = Axon_Count[cou_A];
        }

        delete[] Axons;
        Axons = NULL;

        delete[] Axon_Count;
        Axon_Count = NULL;

        Axons = new c_Node ** [p_Count];
        Axon_Count = new int [p_Count];

        for (int cou_A = 0; cou_A < Axon_Hillock_Count; cou_A++)
        {
            Axons[cou_A] = tmp_Axons[cou_A];
            tmp_Axons[cou_A] = NULL;

            Axon_Count[cou_A] = tmp_Axon_Count[cou_A];
        }

        delete[] tmp_Axons;
        tmp_Axons = NULL;

        delete[] tmp_Axon_Count;
        tmp_Axon_Count = NULL;

        for (int cou_A = Axon_Hillock_Count; cou_A < p_Count; cou_A++)
        {
            Axons[cou_A] = NULL;
            Axon_Count[cou_A] = 0;
        }

        Axon_Hillock_Count = p_Count;
    }

    //Adds an axon to the axon list at the given index, if the index doesn't exist then exist it with resize_Axon_Hillocks()
    void add_Axon_Index(c_Node* p_Axon, int p_Index)
    {
        if (p_Index >= Axon_Hillock_Count) { resize_Axon_Hillocks(p_Index + 1); }

        c_Node** tmp_Axons;
        tmp_Axons = new c_Node * [Axon_Count[p_Index]];
        for (int cou_A = 0; cou_A < Axon_Count[p_Index]; cou_A++)
        {
            tmp_Axons[cou_A] = Axons[p_Index][cou_A];
            Axons[p_Index][cou_A] = NULL;
        }

        delete[] Axons[p_Index];
        Axons[p_Index] = NULL;

        Axons[p_Index] = new c_Node * [Axon_Count[p_Index] + 1];

        for (int cou_A = 0; cou_A < Axon_Count[p_Index]; cou_A++)
        {
            Axons[p_Index][cou_A] = tmp_Axons[cou_A];
            tmp_Axons[cou_A] = NULL;
        }
        delete[] tmp_Axons;
        tmp_Axons = NULL;

        Axons[p_Index][Axon_Count[p_Index]] = p_Axon;
        Axon_Count[p_Index]++;
    }

    //Sets the dendrites of the node.
    //This assumes the node has no dendrites yet, if it does you be dangling and jangling
    void set_Dendrites(c_Node** p_Dendrites, int p_Count)
    {
        Dendrites = new c_Node * [p_Count];

        for (int cou_D = 0; cou_D < p_Count; cou_D++)
        {
            Dendrites[cou_D] = p_Dendrites[cou_D];
        }

        Dendrite_Count = p_Count;
    }

    //Searches the axons to see if an upper tier connection exists.
    //This is always called from the first leg, that is why we separate _F from normal.
    c_Node* does_Upper_Tier_Connection_Exist(c_Node** p_Nodes, int p_Count)
    {
        if (Axon_Hillock_Count > 0)
        {
            //std::cout << "\n Axon_Count[0]: " << Axon_Count[0];

            //Check the axons on the right side for the 
            for (int cou_A = 0; cou_A < Axon_Count[0]; cou_A++)
            {
                //std::cout << "\n   Axon_F[" << cou_A << "] " << Axons[0][cou_A];
                if (Axons[0][cou_A]->does_Lower_Connection_Exist(p_Nodes, p_Count))
                {
                    //std::cout << "\n >>>>>>>>>>>>>Foundit: " << Axons[0][cou_A];
                    return Axons[0][cou_A];
                }
            }
        }
        return NULL;
    }

    //Checks if the given node matches a dendrite on the right leg.
    bool does_Lower_Connection_Exist(c_Node ** p_Nodes, int p_Count)
    {
        if (Dendrite_Count != p_Count) { return 0; }

        //Check the lower connections to see if they match.
        for (int cou_C = 1; cou_C < p_Count; cou_C++)
        {
            if (Dendrites[cou_C] != p_Nodes[cou_C]) { return 0; }
        }
        return 1;
    }

    //Binds a node to a quanta of data, the state of the input.
    void bind_State(uint64_t p_State)
    {
        State = p_State;
    }

    //Initiates a backpropagation that outputs the pattern represented by this node.
    void bp_O()
    {
        std::cout << "<=- " << NID << " ";
        //If a left leg exists then initiate a backpropagation along it, then along the right side.
        if (Dendrite_Count != 0)
        {
            std::cout << "*";
            if (Dendrites[0] != NULL)
            {
                //If not a treetop then call _F
                if ((Dendrites[0]->Type != 2) && (Dendrites[0]->Type != 3))
                {
                    Dendrites[0]->bp_F();
                }

                //If the dendritically connected node is a treetop then call the _O on it.
                if ((Dendrites[0]->Type == 2) || (Dendrites[0]->Type == 3))
                {
                    Dendrites[0]->bp_O();
                }
            }

            for (int cou_D = 1; cou_D < Dendrite_Count; cou_D++)
            {
                if (Dendrites[cou_D] != NULL)
                {
                    //If not a treetop then call bp_M()
                    if ((Dendrites[cou_D]->Type != 2) && (Dendrites[cou_D]->Type != 3))
                    {
                        Dendrites[cou_D]->bp_M();
                        continue;
                    }

                    //If the dendritically connected node is a treetop then call the bp_O on it.
                    if ((Dendrites[cou_D]->Type == 2) || (Dendrites[cou_D]->Type == 3))
                    {
                        Dendrites[cou_D]->bp_O();
                    }
                }
            }
        }
        else
        {
            //Output the state
            //std::cout << " <" << NID << " :: " << State << "> ";

            //u_Tmp tmp_State; tmp_State.U = State;
            //std::cout << " [ " << tmp_State.D << ", " << State << " ] ";
            std::cout << " [ " << char(State) << ", " << State << " ] ";
        }
        std::cout << " -=>";
    }

    //bp_Output the left node.
    void bp_F()
    {
        //If a left leg exists then initiate a backpropagation along it, then along the right side.
        if (Dendrite_Count != 0)
        {
            if (Dendrites[0] != NULL) { Dendrites[0]->bp_F(); }
            for (int cou_D = 1; cou_D < Dendrite_Count; cou_D++)
            {
                if (Dendrites[cou_D] != NULL) 
                {
                    Dendrites[cou_D]->bp_M();
                }
            }
        }
        else
        {
            //Output the state
            //std::cout << " <" << NID << " :: " << State << "> ";
            u_Tmp tmp_State; tmp_State.U = State;
            //std::cout << " [ " << tmp_State.D << ", " << State << " ] ";
            std::cout << " [ " << char(State) << ", " << State << " ] ";
        }
    }

    //bp_Output the other nodes, M stands for many.
    void bp_M()
    {
        //If a left leg exists then initiate a backpropagation along it, then along the right side.
        if (Dendrite_Count != 0)
        {
            for (int cou_D = 1; cou_D < Dendrite_Count; cou_D++)
            {
                if (Dendrites[cou_D] != NULL)
                {
                    Dendrites[cou_D]->bp_M();
                }
            }
        }
        else
        {
            //Output the state
            //std::cout << " <" << NID << " :: " << State << "> ";
            u_Tmp tmp_State; tmp_State.U = State;
            //std::cout << " [ " << tmp_State.D << ", " << State << " ] ";
            std::cout << " [ " << char(State) << ", " << State << " ] ";
        }
    }

    


    //Initiates a backpropagation that 
    void bp_Trace_O(c_Linked_List_Handler * p_LL)
    {
        //---std::cout << "\n bp_Trace_O <=- " << NID << " ";
        //If a left leg exists then initiate a backpropagation along it, then along the right side.
        if (Dendrite_Count != 0)
        {
            //---std::cout << "*";
            if (Dendrites[0] != NULL)
            {
                //If not a treetop then call _F
                if ((Dendrites[0]->Type != 2) && (Dendrites[0]->Type != 3))
                {
                    Dendrites[0]->bp_Trace_F(p_LL);
                }

                //If the dendritically connected node is a treetop then call the _O on it.
                if ((Dendrites[0]->Type == 2) || (Dendrites[0]->Type == 3))
                {
                    Dendrites[0]->bp_Trace_O(p_LL);
                }
            }

            for (int cou_D = 1; cou_D < Dendrite_Count; cou_D++)
            {
                if (Dendrites[cou_D] != NULL)
                {
                    //If not a treetop then call bp_M()
                    if ((Dendrites[cou_D]->Type != 2) && (Dendrites[cou_D]->Type != 3))
                    {
                        Dendrites[cou_D]->bp_Trace_M(p_LL);
                        continue;
                    }

                    //If the dendritically connected node is a treetop then call the bp_O on it.
                    if ((Dendrites[cou_D]->Type == 2) || (Dendrites[cou_D]->Type == 3))
                    {
                        Dendrites[cou_D]->bp_Trace_O(p_LL);
                    }
                }
            }
        }
        else
        {
            //Output the state
            std::cout << " <" << NID << " :: " << State << "> ";
            p_LL->new_LL(State);
        }
        //---std::cout << " -=>";
    }

    //bp_Output the left node.
    void bp_Trace_F(c_Linked_List_Handler* p_LL)
    {
        //If a left leg exists then initiate a backpropagation along it, then along the right side.
        if (Dendrite_Count != 0)
        {
            if (Dendrites[0] != NULL) { Dendrites[0]->bp_Trace_F(p_LL); }
            for (int cou_D = 1; cou_D < Dendrite_Count; cou_D++)
            {
                if (Dendrites[cou_D] != NULL) 
                {
                    Dendrites[cou_D]->bp_Trace_M(p_LL);
                }
            }
        }
        else
        {
            //Output the state
            //---std::cout << " <" << NID << " :: " << State << "> ";
            p_LL->new_LL(State);
        }
    }

    //bp_Output the other nodes, M stands for many.
    void bp_Trace_M(c_Linked_List_Handler* p_LL)
    {
        //If a left leg exists then initiate a backpropagation along it, then along the right side.
        if (Dendrite_Count != 0)
        {
            for (int cou_D = 1; cou_D < Dendrite_Count; cou_D++)
            {
                if (Dendrites[cou_D] != NULL)
                {
                    Dendrites[cou_D]->bp_Trace_M(p_LL);
                }
            }
        }
        else
        {
            //Output the state
            //---std::cout << " <" << NID << " :: " << State << "> ";
            p_LL->new_LL(State);
        }
    }


    void output_Node_Raw()
    {
        std::cout << "\n Node_ID (NID): " << NID << " ";
        std::cout << " RC: " << RC;
        std::cout << " Type: " << Type;
        std::cout << " State { " << State << " }";
        std::cout << " --- Dendrites: ";
        for (int cou_D = 0; cou_D < Dendrite_Count; cou_D++)
        {
            std::cout << " [" << cou_D << "] " << Dendrites[cou_D]->NID;
        }
        std::cout << " --- Axon_Hillock_Count: " << Axon_Hillock_Count;
        for (int cou_H = 0; cou_H < Axon_Hillock_Count; cou_H++)
        {
            std::cout << " Axons[" << cou_H << "] <" << Axon_Count[cou_H] << ">: ";
            for (int cou_A = 0; cou_A < Axon_Count[cou_H]; cou_A++)
            {
                std::cout << " [" << cou_A << "] " << Axons[cou_H][cou_A]->NID;
            }
        }
    }

    void output_Node_Char()
    {
        std::cout << char(this);
    }
};