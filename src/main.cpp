///////////////////////////////////////////////////
//
//	Hamish Carr
//	January, 2018
//
//	------------------------
//	main.cpp
//	------------------------
//	
///////////////////////////////////////////////////
#include "GeometricSurfaceFaceDS.h"

#include <stdio.h>

int main(int argc, char **argv)
	{ // main()
    // the geometric surface
	GeometricSurfaceFaceDS surface;

	// check the args to make sure there's an input file
	if (argc == 2)
    {
        printf("Now, dont accept only two arguments!\n");
        exit(0);
    }
    else if(argc ==3)
    {
        if (!surface.ReadFileTriangleSoup(argv[1]))
        { // surface read failed
            printf("Read failed for file %s\n", argv[1]);
            exit(0);
        } // surface read failed

        if('f'==*argv[2])
        {
            printf("Write a f2f file(.face)\n");
            surface.write_f2f_file(argv[1]);
            surface.manifold_test();
            surface.compute_genus();
        }
        else if('r'==*argv[2])
        {
            printf("Read a f2f file(.face)\n");
            surface.read_f2f_file(argv[1]);
            surface.manifold_test();
            surface.compute_genus();
        }
        else if('d'==*argv[2])
        {
            printf("Write a directed edge file(.diredge)\n");
            surface.write_diredge_file(argv[1]);
            surface.manifold_test();
            surface.compute_genus();
        }
        else{printf("Usage: %s\n"
                    "'f' write f2f index file\n"
                    "'r' read f2f index file\n"
                    "'d' write directed edge index file\n"
                    "and each option could do manifold test and genus computation!\n", argv[1]);exit(0); }
        // a function to out the face file
    }
	else 
		{ // too many parameters 
        printf("Usage: %s filename\n"
               "'f' write f2f index file\n"
               "'r' read f2f index file\n"
               "'d' write directed edge index file\n"
               "and each option could do manifold test and genus computation!\n", argv[1] );
		exit (0);
		} // too many parameters 

	// paranoid return value
	exit(0);
	} // main()
