

class c_Sim
{
public:

	int Iteration;

	int O2;
	int O2_Delta;

	int Temp;
	int Temp_Delta;

	int O2_Pump;
	int Heater;

	c_Sim()
	{
		Iteration = 0;

		O2 = 5;
		O2_Delta = 1;

		Temp = 5;
		Temp_Delta = 1;

		O2_Pump = 0;
		Heater = 0;
	}

	// Afferent Sensors - Goals
	// O2      | -1/iteration //Breathing   | Goal: 5 | G:5 Y:(4 & 6)      R: (< 4 & > 6)  | Priority: 2  
	// Temp    | -2/iteration //Winter      | Goal: 8 | G:8 Y:(6-7 & 9-10) R: (< 6 & > 10) | Priority: 2  

	// Actuator Systems - Feedback
	// O2 Pump | +4/Iteration [O2]          | -1/Iteration [Temperature] | if (O2 < 5) (Heater = 1) else (Heater = 0)
	// Heater  | +3/Iteration [Temperature] |                            | if (Temp < 8) (Heater = 1) else (Heater = 0)

	int iterate(int p_O2_Pump = -1, int p_Heater = -1)
	{
		Iteration++;

		//Environmental
		int tmp_O2 = O2;
		O2 -= 1; //Breathing

		int tmp_Temp = Temp;
		Temp -= 2; //Winter

		//Calculate actuator effects.
		if (O2_Pump) { O2 += 4; Temp -= 1; } //O2 Pump
		if (Heater) { Temp += 3; } //Heater

		//Calculate the environmental changes based on the previous iterations.
		if (O2 < tmp_O2) { O2_Delta = 0; }
		if (O2 == tmp_O2) { O2_Delta = 1; }
		if (O2 > tmp_O2) { O2_Delta = 2; }

		if (Temp < tmp_Temp) { Temp_Delta = 0; }
		if (Temp == tmp_Temp) { Temp_Delta = 1; }
		if (Temp > tmp_Temp) { Temp_Delta = 2; }

		if ((p_Heater != -1) && (p_O2_Pump != -1))
		{
			Heater = p_Heater;
			O2_Pump = p_O2_Pump;
		}
		else
		{
			//Calculate the actuator state.
			if (tmp_Temp < 8) { Heater = 1; }
			if (tmp_O2 < 5) { O2_Pump = 1; }

			if (tmp_Temp >= 8) { Heater = 0; }
			if (tmp_O2 >= 5) { O2_Pump = 0; }
		}

		return Iteration;
	}

	void output()
	{
		std::cout << "\n";
		std::cout << Iteration;
		std::cout << "\nencode 0 set_input 0 O2 " << O2;
		std::cout << "\nencode 0 set_input 0 O2_Delta " << O2_Delta;

		std::cout << "\nencode 0 set_input 0 Temp " << Temp;
		std::cout << "\nencode 0 set_input 0 Temp_Delta " << Temp_Delta;

		std::cout << "\nencode 0 set_input 0 O2_Pump " << O2_Pump;
		std::cout << "\nencode 0 set_input 0 Heater " << Heater;
	}

	void output_F(std::string p_FName)
	{
		std::ofstream tmp_F;

		tmp_F.open(p_FName, std::ios::app);


		tmp_F << "\n";
		tmp_F << "\noutput_newline";
		tmp_F << "\nset_input 0 ar_O2 " << O2 << " /end/ encode 0 gather_treetop_node 0 ";
		tmp_F << "\nset_input 1 ac_O2_Delta " << O2_Delta << " /end/ encode 1 gather_treetop_node 1 ";
		tmp_F << "\nset_input 2 em_O2_Pump " << O2_Pump << " /end/ encode 2 gather_treetop_node 2 ";

		tmp_F << "\nset_input 3 ar_Temp " << Temp << " /end/ encode 3 gather_treetop_node 3 ";
		tmp_F << "\nset_input 4 ac_Temp_Delta " << Temp_Delta << " /end/ encode 4 gather_treetop_node 4 ";
		tmp_F << "\nset_input 5 em_Heater " << Heater << " /end/ encode 5 gather_treetop_node 5 ";

		tmp_F.close();
	}
};

 

class c_Map_Sim
{
	char** Map;

	int Width;

	int Height;

	int Frame;

	std::string Filename;

public:

	c_Map_Sim(int p_Width, int p_Height)
	{
		Map = NULL;

		Filename = "./Scripts/";

		new_Map(p_Width, p_Height);
	}

	void new_Map(int p_Width, int p_Height)
	{
		Width = p_Width;
		Height = p_Height;

		std::cout << "\n Creating Map(" << Width << ", " << Height << ")";
		Map = new char*[Width];

		for (int cou_X = 0; cou_X < Width; cou_X++)
		{
			Map[cou_X] = new char[Height];

			for (int cou_Y = 0; cou_Y < Height; cou_Y++)
			{
				Map[cou_X][cou_Y] = ' ';
				std::cout << Map[cou_X][cou_Y];
			}
		}
		std::cout << "\n Finished Map";
	}

	void set_Pixel(int p_X, int p_Y, char p_Pixel)
	{
		Map[p_X][p_Y] = p_Pixel;
	}

	void view_Map()
	{
		for (int cou_X = 0; cou_X < Width; cou_X++)
		{
			std::cout << "\n[";
			for (int cou_Y = 0; cou_Y < Height; cou_Y++)
			{
				//std::cout << int(Map[cou_X][cou_Y]) << " ";
				std::cout << Map[cou_X][cou_Y] << " ";
			}
			std::cout << "]";
		}
		std::cout << "\n";
	}
};