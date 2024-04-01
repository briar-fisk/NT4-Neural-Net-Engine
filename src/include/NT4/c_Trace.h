


//This class serves as the user interface for the actuator I/O
class c_Trace
{
public:

    //Data is stored as an array of uint64_t
    uint64_t* Pattern;
    double Charge;
    double RC;
    c_Node* Treetop;

    //The depth of the data.
    int Depth;

    c_Trace()
    {
        Pattern = NULL;
        reset();
    }

    void set_Depth(int p_Depth)
    {
        //If Data is NULL then everything else should be as well.
        //This assumes that if Pattern is not NULL then it is properly setup as an array.
        if (Pattern != NULL)
        {
            delete[] Pattern;
            Pattern = NULL;
        }

        Depth = p_Depth;

        Pattern = new uint64_t[Depth];

        wipe_Data();
    }

    //This leaves the depth of the pattern alone, useful if you can define one array and reuse it without having to redeclare and resize the array.
    //Used after set_Depth to intialize the new array.
    void wipe_Data()
    {
        for (int cou_Index = 0; cou_Index < Depth; cou_Index++)
        {
            Pattern[cou_Index] = 0;
        }

        Charge = 0.0;
        RC = 0.0;
        Treetop = NULL;
    }


    //Reset it if need be.
    void reset()
    {
        wipe_Data();

        if (Pattern != NULL)
        {
            delete[] Pattern;
            Pattern = NULL;
        }

        Pattern = NULL;

        Charge = 0.0;
        RC = 0.0;
        Treetop = NULL;

        Depth = 0;
    }

    //Accepts an input and updates the correct index with it
    void set_Pattern_Index(uint64_t p_Value, int p_Index = -1) //p_Index comes after since it may be 0
    {
        if (p_Index == -1)
        {
            p_Index = Depth - 1;
        }

        //First get the concrete value.
        Pattern[p_Index] = p_Value;
    }

    //Sets the Charge
    void set_Charge(double p_Charge)
    {
        Charge = p_Charge;
    }

    //Sets the RC
    void set_RC(double p_RC)
    {
        RC = p_RC;
    }

    //Sets the Charge
    void set_Treetop(c_Node * p_Treetop)
    {
        Treetop = p_Treetop;
    }

    //Accepts an array, resets the data, then reads the array into the data then sets the depth.
    void set_Pattern_Array(uint64_t* p_Pattern_Array, int p_Depth)
    {
        reset();

        //Initialize the wiped input array and copy the data to it from the arguments.
        set_Depth(p_Depth);

        for (int cou_Index = 0; cou_Index < Depth; cou_Index++)
        {
            Pattern[cou_Index] = p_Pattern_Array[cou_Index];
        }
    }

    //Returns the value.
    uint64_t get_Pattern_Index(int p_Index = -1)
    {
        if (p_Index == -1)
        {
            p_Index = Depth - 1;
        }

        return Pattern[p_Index];
    }

    double get_Charge()
    {
        return Charge;
    }

    double get_RC()
    {
        return RC;
    }

    c_Node * get_Treetop()
    {
        return Treetop;
    }

    //This shifts the output from current to 0.
    void shift_Data()
    {
        //Shift each array one index towards [0].
        for (int cou_Index = 0; cou_Index < (Depth - 1); cou_Index++)
        {
            Pattern[cou_Index] = Pattern[cou_Index + 1];
        }

        //Set the current to 0.0 in preparation for input.
        Pattern[Depth - 1] = 0;
    }

    //Outputs the data
    void output(int p_Type)
    {
        uint64_t tmp_Tt_NID = 0;
        if (Treetop != NULL) { tmp_Tt_NID = Treetop->NID; }
        std::cout << "\n --  Charge: " << Charge << " RC: " << RC << " Treetop: " << tmp_Tt_NID << " Pattern: ";
        
        for (int cou_Index = 0; cou_Index < Depth; cou_Index++)
        {
            if (p_Type == 0){ std::cout << " " << char(Pattern[cou_Index]); }
            if (p_Type == 1){ std::cout << " " << Pattern[cou_Index]; }
        }
    }
};
















//This class serves as the user interface for the actuator I/O
class c_2D_Trace
{
public:

    //Data is stored as an array of uint64_t
    uint64_t** Pattern;
    double Charge;
    double RC;
    c_Node* Treetop;

    //The depth of the data.
    int Depth_X;
    int Depth_Y;

    c_2D_Trace()
    {
        Pattern = NULL;
        Depth_X = 0;
        Depth_Y = 0;
        reset();
    }

    void set_Depth(int p_Depth_X, int p_Depth_Y)
    {
        //If Data is NULL then everything else should be as well.
        //This assumes that if Pattern is not NULL then it is properly setup as an array.
        if (Pattern != NULL)
        {
            for (int cou_X = 0; cou_X < Depth_X; cou_X++)
            {
                delete[] Pattern[cou_X];
                Pattern[cou_X] = NULL;
            }

            delete[] Pattern;
            Pattern = NULL;
        }

        Depth_X = p_Depth_X;
        Depth_Y = p_Depth_Y;

        Pattern = new uint64_t * [Depth_X];

        for (int cou_X = 0; cou_X < Depth_X; cou_X++)
        {
            //It pains me not to wipe the data here, but we do it in wipe_Data
            Pattern[cou_X] = new uint64_t[Depth_Y];
        }

        wipe_Data();
    }

    //This leaves the depth of the pattern alone, useful if you can define one array and reuse it without having to redeclare and resize the array.
    //Used after set_Depth to intialize the new array.
    void wipe_Data()
    {
        for (int cou_X = 0; cou_X < Depth_X; cou_X++)
        {
            for (int cou_Y = 0; cou_Y < Depth_Y; cou_Y++)
            {
                Pattern[cou_X][cou_Y] = 0;
            }
        }

        Charge = 0.0;
        RC = 0.0;
        Treetop = NULL;
    }


    //Reset it if need be.
    void reset()
    {
        if (Pattern != NULL)
        {
            for (int cou_X = 0; cou_X < Depth_X; cou_X++)
            {
                if (Pattern[cou_X] != NULL)
                {
                    delete[] Pattern[cou_X];
                    Pattern[cou_X] = NULL;
                }
            }
            delete[] Pattern;
            Pattern = NULL;
        }

        Charge = 0.0;
        RC = 0.0;
        Treetop = NULL;

        Depth_X = 0;
        Depth_Y = 0;
    }

    //Accepts an input and updates the correct index with it
    void set_Pattern_Index(uint64_t p_Value, int p_X, int p_Y)
    {
        Pattern[p_X][p_Y] = p_Value;
    }

    //Sets the Charge
    void set_Charge(double p_Charge)
    {
        Charge = p_Charge;
    }

    //Sets the RC
    void set_RC(double p_RC)
    {
        RC = p_RC;
    }

    //Sets the associated node.
    void set_Treetop(c_Node* p_Treetop)
    {
        Treetop = p_Treetop;
    }

    //Accepts an array, resets the data, then reads the array into the data then sets the depth.
    void set_Pattern_Array(uint64_t** p_Pattern_Array, int p_Depth_X, int p_Depth_Y)
    {
        reset();

        //Initialize the wiped input array and copy the data to it from the arguments.
        set_Depth(p_Depth_X, p_Depth_Y);

        for (int cou_X = 0; cou_X < Depth_X; cou_X++)
        {
            for (int cou_Y = 0; cou_Y < Depth_Y; cou_Y++)
            {
                Pattern[cou_X][cou_Y] = p_Pattern_Array[cou_X][cou_Y];
            }
        }
    }

    //Returns the value.
    uint64_t get_Pattern_Index(int p_X, int p_Y)
    {
        return Pattern[p_X][p_Y];
    }

    double get_Charge()
    {
        return Charge;
    }

    double get_RC()
    {
        return RC;
    }

    c_Node* get_Treetop()
    {
        return Treetop;
    }

    //This shifts the output from current to 0.
    void shift_Data()
    {
        //Shift each array one index towards [0] on the x axis.
        for (int cou_X = 0; cou_X < (Depth_X - 1); cou_X++)
        {
            for (int cou_Y = 0; cou_Y < Depth_Y; cou_Y++)
            {
                Pattern[cou_X][cou_Y] = Pattern[cou_X + 1][cou_Y];
            }
        }

        //Set the current to 0.0 in preparation for input.
        for (int cou_Y = 0; cou_Y < Depth_Y; cou_Y++)
        {
            Pattern[Depth_X - 1][cou_Y] = 0;
        }
    }

    //Outputs the data
    void output(int p_Type)
    {
        uint64_t tmp_Tt_NID = 0;
        if (Treetop != NULL) { tmp_Tt_NID = Treetop->NID; }
        std::cout << "\n --  Charge: " << Charge << " RC: " << RC << " Treetop: " << tmp_Tt_NID << " Pattern: ";

        for (int cou_X = 0; cou_X < Depth_X; cou_X++)
        {
            std::cout << "\n";
            for (int cou_Y = 0; cou_Y < Depth_Y; cou_Y++)
            {
                if (p_Type == 0) { std::cout << " " << char(Pattern[cou_X][cou_Y]); }
                if (p_Type == 1) { std::cout << " " << Pattern[cou_X][cou_Y]; }
            }
        }
    }
};























//This class serves as the user interface for the actuator I/O
class c_3D_Trace
{
public:

    //Data is stored as an array of uint64_t
    uint64_t*** Pattern;
    double Charge;
    double RC;
    c_Node* Treetop;

    //The depth of the data.
    int Depth_X;
    int Depth_Y;
    int Depth_Z;

    c_3D_Trace()
    {
        Pattern = NULL;
        Depth_X = 0;
        Depth_Y = 0;
        Depth_Z = 0;
        reset();
    }

    void set_Depth(int p_Depth_X, int p_Depth_Y, int p_Depth_Z)
    {
        //If Data is NULL then everything else should be as well.
        //This assumes that if Pattern is not NULL then it is properly setup as an array.
        if (Pattern != NULL)
        {
            for (int cou_X = 0; cou_X < Depth_X; cou_X++)
            {
                for (int cou_Y = 0; cou_Y < Depth_Y; cou_Y++)
                {
                    delete[] Pattern[cou_X][cou_Y];
                    Pattern[cou_X][cou_Y] = NULL;
                }
                delete[] Pattern[cou_X];
                Pattern[cou_X] = NULL;
            }
            delete[] Pattern;
            Pattern = NULL;
        }

        Depth_X = p_Depth_X;
        Depth_Y = p_Depth_Y;
        Depth_Z = p_Depth_Z;

        Pattern = new uint64_t ** [Depth_X];

        for (int cou_X = 0; cou_X < Depth_X; cou_X++)
        {
            Pattern[cou_X] = new uint64_t*[Depth_Y];

            for (int cou_Y = 0; cou_Y < Depth_Y; cou_Y++)
            {
                //It pains me not to wipe the data here, but we do it in wipe_Data
                Pattern[cou_X][cou_Y] = new uint64_t[Depth_Z];
            }
        }

        wipe_Data();
    }

    //This leaves the depth of the pattern alone, useful if you can define one array and reuse it without having to redeclare and resize the array.
    //Used after set_Depth to intialize the new array.
    void wipe_Data()
    {
        for (int cou_X = 0; cou_X < Depth_X; cou_X++)
        {
            for (int cou_Y = 0; cou_Y < Depth_Y; cou_Y++)
            {
                for (int cou_Z = 0; cou_Z < Depth_Z; cou_Z++)
                {
                    Pattern[cou_X][cou_Y][cou_Z] = 0;
                }
            }
        }

        Charge = 0.0;
        RC = 0.0;
        Treetop = NULL;
    }


    //Reset it if need be.
    void reset()
    {
        wipe_Data();

        if (Pattern != NULL)
        {
            for (int cou_X = 0; cou_X < Depth_X; cou_X++)
            {
                if (Pattern[cou_X] != NULL)
                {
                    for (int cou_Y = 0; cou_Y < Depth_Y; cou_Y++)
                    {
                        if (Pattern[cou_X][cou_Y] != NULL)
                        {
                            delete[] Pattern[cou_X][cou_Y];
                            Pattern[cou_X][cou_Y] = NULL;
                        }
                    }
                    delete[] Pattern[cou_X];
                    Pattern[cou_X] = NULL;
                }
            }
            delete[] Pattern;
            Pattern = NULL;
        }


        Charge = 0.0;
        RC = 0.0;
        Treetop = NULL;

        Depth_X = 0;
        Depth_Y = 0;
        Depth_Z = 0;
    }

    //Accepts an input and updates the correct index with it
    void set_Pattern_Index(uint64_t p_Value, int p_X, int p_Y, int p_Z)
    {
        Pattern[p_X][p_Y][p_Z] = p_Value;
    }

    //Sets the Charge
    void set_Charge(double p_Charge)
    {
        Charge = p_Charge;
    }

    //Sets the RC
    void set_RC(double p_RC)
    {
        RC = p_RC;
    }

    //Sets the associated node.
    void set_Treetop(c_Node* p_Treetop)
    {
        Treetop = p_Treetop;
    }

    //Accepts an array, resets the data, then reads the array into the data then sets the depth.
    void set_Pattern_Array(uint64_t*** p_Pattern_Array, int p_Depth_X, int p_Depth_Y, int p_Depth_Z)
    {
        reset();

        //Initialize the wiped input array and copy the data to it from the arguments.
        set_Depth(p_Depth_X, p_Depth_Y, p_Depth_Z);

        for (int cou_X = 0; cou_X < Depth_X; cou_X++)
        {
            for (int cou_Y = 0; cou_Y < Depth_Y; cou_Y++)
            {
                for (int cou_Z = 0; cou_Z < Depth_Z; cou_Z++)
                {
                    Pattern[cou_X][cou_Y][cou_Z] = p_Pattern_Array[cou_X][cou_Y][cou_Z];
                }
            }
        }
    }

    //Returns the value.
    uint64_t get_Pattern_Index(int p_X, int p_Y, int p_Z)
    {
        return Pattern[p_X][p_Y][p_Z];
    }

    double get_Charge()
    {
        return Charge;
    }

    double get_RC()
    {
        return RC;
    }

    c_Node* get_Treetop()
    {
        return Treetop;
    }

    //This shifts the output from current to 0.
    void shift_Data()
    {
        //Shift each array one index towards [0] on the x axis.
        for (int cou_X = 0; cou_X < (Depth_X - 1); cou_X++)
        {
            for (int cou_Y = 0; cou_Y < Depth_Y; cou_Y++)
            {
                for (int cou_Z = 0; cou_Z < Depth_Z; cou_Z++)
                {
                    Pattern[cou_X][cou_Y][cou_Z] = Pattern[cou_X + 1][cou_Y][cou_Z];
                }
            }
        }

        //Set the current to 0.0 in preparation for input.
        for (int cou_Y = 0; cou_Y < Depth_Y; cou_Y++)
        {
            for (int cou_Z = 0; cou_Z < Depth_Z; cou_Z++)
            {
                Pattern[Depth_X - 1][cou_Y][cou_Z] = 0;
            }
        }
    }

    //Outputs the data
    void output(int p_Type)
    {
        uint64_t tmp_Tt_NID = 0;
        if (Treetop != NULL) { tmp_Tt_NID = Treetop->NID; }
        std::cout << "\n --  Charge: " << Charge << " RC: " << RC << " Treetop: " << tmp_Tt_NID << " Pattern: ";

        for (int cou_X = 0; cou_X < Depth_X; cou_X++)
        {
            std::cout << "\n";
            for (int cou_Y = 0; cou_Y < Depth_Y; cou_Y++)
            {
                std::cout << "\n";
                for (int cou_Z = 0; cou_Z < Depth_Z; cou_Z++)
                {
                    if (p_Type == 0) { std::cout << " " << char(Pattern[cou_X][cou_Y][cou_Z]); }
                    if (p_Type == 1) { std::cout << " " << Pattern[cou_X][cou_Y][cou_Z]; }
                }
            }
        }
    }
};