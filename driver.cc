#include "central2d.h"
#include "shallow2d.h"
#include "minmod.h"
#include "meshio.h"

#ifdef _OPENMP
#include <omp.h>
#endif

#include <string>
#include <cmath>
#include <cstring>
#include <cstdlib>
#include <unistd.h>


//ldoc on
/**
 * # Driver routines
 *  
 * We use a fairly simple command-line driver to launch this simulation.
 * A better way to do this is to use a scripting language to set up the
 * simulation; Python is a popular choice, though I prefer Lua for many
 * things (not least because it is an easy build).  I may add that
 * capability later; for the moment, it's useful to have a simple
 * command-line interface that ought to run most anywhere.
 * 
 * For the driver, we need to put everything together: we're running
 * a `Central2D` solver for the `Shallow2D` physics with a `MinMod`
 * limiter:
 */

typedef Central2D< Shallow2D, MinMod<Shallow2D::real> > Sim;

/**
 * ## Initial states
 * 
 * Our default problem is a circular dam break problem; the other
 * interesting problem is the wave problem (a wave on a constant
 * flow, starting off smooth and developing a shock in finite time).
 * The pond and river examples should do nothing interesting at all
 * if the numerical method is coded right.
 */

// Circular dam break problem



/**
 * ## Main driver
 * 
 * Our main driver uses the `getopt` library to parse options,
 * then runs a simulation, writing results to an output file
 * for postprocessing.
 */

int main(int argc, char** argv)
{

    double width = 2.0;
    double ftime = 0.01;
    int    frames = 50;
    int nx = 200;
    
    int c;
    /*
    extern char* optarg;
    while ((c = getopt(argc, argv, "hi:o:n:w:F:f:")) != -1) {
        switch (c) {
        case 'h':
            fprintf(stderr,
                    "%s\n"
                    "\t-h: print this message\n"
                    "\t-i: initial conditions (%s)\n"
                    "\t-o: output file name (%s)\n"
                    "\t-n: number of cells per side (%d)\n"
                    "\t-w: domain width in cells (%g)\n"
                    "\t-f: time between frames (%g)\n"
                    "\t-F: number of frames (%d)\n",
                    argv[0], ic.c_str(), fname.c_str(), 
                    nx, width, ftime, frames);
            return -1;
        case 'i':  ic     = optarg;          break;
        case 'o':  fname  = optarg;          break;
        case 'n':  nx     = atoi(optarg);    break;
        case 'w':  width  = atof(optarg);    break;
        case 'f':  ftime  = atof(optarg);    break;
        case 'F':  frames = atoi(optarg);    break;
        default:
            fprintf(stderr, "Unknown option (-%c)\n", c);
            return -1;
        }
    }

    void (*icfun)(Sim::vec& u, double x, double y) = dam_break;
    if (ic == "dam_break") {
        icfun = dam_break;
    } else if (ic == "pond") {
        icfun = pond;
    } else if (ic == "river") {
        icfun = river;
    } else if (ic == "wave") {
        icfun = wave;
    } else {
        fprintf(stderr, "Unknown initial conditions\n");
    }
    */
    double t0 = omp_get_wtime();
    
    Sim sim(width,width, NX,NX);
    SimViz<Sim> viz("dam_break", sim);
    sim.init();
    sim.solution_check();
    viz.write_frame();
    for (int i = 0; i < frames; ++i) {
#ifdef _OPENMP
        
        sim.run(ftime);

#else
        sim.run(ftime);
#endif
        sim.solution_check();
        viz.write_frame();
    }
    double t1 = omp_get_wtime();
    printf("Time: %e\n", t1-t0);
}
