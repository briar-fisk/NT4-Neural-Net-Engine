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

int main()
{
    NT4::c_Construct_Text_Server Serverman;
    //generate_Test_Data();
    //system("PAUSE");

    Serverman.run();

	return 0;
}
