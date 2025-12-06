#include "DMSA_Controller.h"

int main()
{
	try
	{
		// Run the single comprehensive mixed-type input file
		std::cout << "\n[1/1] Processing 'mixed_input_full.txt'...\n";

		// Initialize controller with the specific input file and a corresponding output file
		DMSA_Controller controller( "mixed_input_full_4.txt", "output_mixed_full.txt" );

		// Execute the parsing and operation engine
		controller.Run();

		std::cout << "\nExecution Complete. Results saved to 'output_mixed_full.txt'\n";
	}
	catch ( const std::exception & e )
	{
		std::cerr << "\nFatal Error: " << e.what() << std::endl;
		return 1;
	}

	return 0;
}