// NT4.cpp : Defines the entry point for the application.
//

#include "NT4.h"


using namespace std;











































                    

//Config.ssv - This file configures the network and sets the initial hyper-parameters. It handles the loading of any node files needed when using pretrained models.






//Input.ssv - This file holds the input data, each row an index

//Output.ssv - This file holds the output data, a table holding traces.

//This isn't for the engine itself, but for the testing of it. To remove in release version.
void generate_Test_Data()
{
    c_Sim Simboi;
    for (int cou_Index = 0; cou_Index < 100; cou_Index++)
    {
        Simboi.iterate();
        Simboi.output();
        Simboi.output_F("Sim_Test_Data.txt");
    }
}

void runmap()
{
    c_Map_Sim Map(50, 50);

    int tmp_X = 0;
    int tmp_Y = 0;
    char tmp_Char = ' ';


    while (1)
    {
        tmp_X = 0;
        tmp_Y = 0;
        tmp_Char = ' ';

        Map.view_Map();

        std::cin >> tmp_X;
        std::cin >> tmp_Y;

        std::cin >> tmp_Char;

        Map.set_Pixel(tmp_X, tmp_Y, tmp_Char);
    }

}

int main(int argc, char** argv)
{
    //runmap();

    std::string tmp_Autoexec = "../autoexec.ssv";
    if (argc > 1) { tmp_Autoexec = argv[1]; }
    NT4::c_Construct_Text_Server Serverman(tmp_Autoexec);

    //generate_Test_Data();
    //system("PAUSE");

    Serverman.run();

	return 0;
}
