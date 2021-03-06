//-*-C++-*-
//---------------------------------------------------------------------------
#ifndef NaCSMH
#define NaCSMH

#include <NaPetri.h>
#include <NaPNSum.h>
#include <NaPNCmp.h>
#include <NaPNGen.h>
#include <NaPNTran.h>
#include <NaPNTime.h>
#include <NaPNStat.h>
#include <NaPNChPt.h>
#include <NaPNFIn.h>
#include <NaPNFOut.h>
#include <NaPNBu21.h>
#include <NaPNBu31.h>
#include <NaPNDely.h>
#include <NaPNRand.h>
#include <NaPNDerv.h>
#include <NaPNCuSu.h>
#include <NaPNWatc.h>
#include <NaNDescr.h>
#include <NaPNNNUn.h>
#include <NaPNSkip.h>
#include <NaPNFill.h>
#include <NaPNTDG.h>
#include <NaPNSS.h>
#include <NaCoFunc.h>
#include <NaPNTchr.h>
#include <NaPNAccu.h>


//---------------------------------------------------------------------------
// Class for traditional control system modelling
class NaControlSystemModel
{
public:/* methods */

    // Create control system with stream of given length in input or with
    // with data files if len=0
    NaControlSystemModel (int len, NaControllerKind ckind);

    // Destroy the object
    virtual ~NaControlSystemModel ();


    ////////////////////
    // Network phases //
    ////////////////////

    // Link the network (tune the net before)
    virtual void        link_net ();

    // Run the network
    virtual NaPNEvent   run_net ();


    //////////////////
    // Overloadable //
    //////////////////

    // Check for user break
    virtual bool        user_break ();

    // Each cycle callback
    virtual void        idle_entry ();

    ////////////////
    // Additional //
    ////////////////

    // Set initial value of control error
    void		set_initial_cerror (const NaVector& v);

    // Set flag of using cummulative sum features
    void		set_cusum_flag (bool use_cusum);

    // Set flag of using training data gather features
    void		set_tdg_flag (bool use_tdg);

    // Set plant function
    void		set_plant_function (NaCombinedFunc& cofunc);

    // Set plant model
    void		set_plant_ss_model (NaStateSpaceModel& ssm);

public:/* data */

    // Main Petri network module
    NaPetriNet      net;

    // Functional Petri network nodes
    NaPNFileInput   setpnt_inp;
    NaPNRandomGen   setpnt_gen;
    NaPNCheckPoint  chkpnt_r;
    NaPNComparator  cmp;
    NaPNCheckPoint  chkpnt_e;
    NaPNTransfer    controller;
    NaPNCheckPoint  chkpnt_u;
    NaPNFileInput   noise_inp;
    NaPNRandomGen   noise_gen;
    NaPNCheckPoint  chkpnt_n;
    NaPetriNode     *p_plant;
    NaPNTransfer    plant_cof;
    NaPNStateSpace  plant_ssm;
    NaPNCheckPoint  chkpnt_y;
    NaPNCheckPoint  chkpnt_ny;
    NaPNSum         onsum;
    NaPNComparator  cmp_e;	// reference minus pure y produces e for MSE
    NaPNStatistics  statan_cerr;// statistics calculation for control error
    NaPNStatistics  statan_r;
    NaPNBus2i1o     bus2;       // (r,e)->NN former
    NaPNBus3i1o     bus3;       // (r,e,se)->NN former
    NaPNAccumulator err_acc;    // accumulate error to estimate its integral
    NaPNDelay       delay;      // (e(i),e(i-1),...e(i-n))->NN delayer-former
    NaPNDerivative  delta_e;    // (1-1/z)*e(k)
    NaPNCuSum       cusum;      // cumulative sum for change point detection
    NaPNFileOutput  cusum_out;  // output of cumulative sum
    NaPNWatcher     dodetect;   // disorder watcher
    NaPNBus2i1o     bus_p;      // ((u,y),e)->NN plant former
    NaPNNNUnit      nnplant;    // NN plant
    NaPNSkip        skip_y;     // skip some y due to u isn't available
    NaPNSkip        skip_u;     // skip some u due to y isn't available
    NaPNSkip        skip_ny;    // skip some not meaning n+y values
    NaPNComparator  iderrcomp;  // identification error computer
    NaPNStatistics  statan_iderr;// identification error statistics
    NaPNDelay       delay_y;    // y -> y(-1), y(-2), ...
    NaPNDelay       delay_u;    // u -> u(-1), u(-2), ...
    NaPNFileOutput  nn_x;       // NN controller input
    NaPNFileOutput  nn_y;       // NN plant output
    NaPNFileOutput  nn_e;       // NN identification error output
    NaPNFill        fill_nn_y;  // fill by zeros nn_y output to fit on_y
    NaPNTrainDataGath trdgath;	// train data gathering
    NaPNFileOutput  tdg_u;      // u series
    NaPNFileOutput  tdg_ny;     // y+n series
    NaPNFileOutput  cerr_fout;  // output statistics of control error (see statan_cerr)
    NaPNFileOutput  iderr_fout; // output statistics of identif. error (see statan_iderr)
    NaPNFileOutput  plant_x;    // Output of plant state (see plant_ssm)
    NaPNSkip        skip_r;     // Skip some r to wait for u and y
    NaPNTeacher     nnpback;    // Backward propagator through nnplant
    NaPNFileOutput  nnp_bpe;    // Backpropagated control error through NNP

private:/* data */

    // Kind of controller
    NaControllerKind	eContrKind;

    // Length of series or 0 for data input
    int			nSeriesLen;

    // Initial value of the control error to start loop evaluation
    NaVector		vInitial;

    // Cummulative sum usage
    bool		bUseCuSum;

    // Training data gather usage
    bool		bUseTDG;

    // Flag of state space plant model
    bool	        bPlantSSM;
};


//---------------------------------------------------------------------------
#endif
