

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
		tmp_F << "\nset_input 0 O2 " << O2 << " /end/ encode 0 gather_treetop_node 0 ";
		tmp_F << "\nset_input 0 O2_Delta " << O2_Delta << " /end/ encode 0 gather_treetop_node 0 ";

		tmp_F << "\nset_input 0 Temp " << Temp << " /end/ encode 0 gather_treetop_node 0 ";
		tmp_F << "\nset_input 0 Temp_Delta " << Temp_Delta << " /end/ encode 0 gather_treetop_node 0 ";

		tmp_F << "\nset_input 0 O2_Pump " << O2_Pump << " /end/ encode 0 gather_treetop_node 0 ";
		tmp_F << "\nset_input 0 Heater " << Heater << " /end/ encode 0 gather_treetop_node 0 ";

		tmp_F.close();
	}
};

 