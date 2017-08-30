#include izhikevich.h

string neuronType="excitatory"; //choice variable excitatory or inhibitory

// Synaptic parameters
float e_rev_gaba,thr;
float tau_ampa,tau_gaba;
float scale_ampa, scale_gaba;

//Synaptic conductances
float g_ampa;
float g_gaba;

// Synaptic currents
float inh_current;
float exc_current;

// Neuron dynamic parameters
float avar;
float bvar;
float cvar;
float dvar;

// Neuron firing variables (voltage and adaptation_var)
float membrane_voltage;
float adaptation_var;

void init(){
	e_rev_gaba = -80e-3;
	tau_ampa = 5e-3;
	tau_gaba = 10e-3;

	avar = 0.02;   // adaptation variable rate constant
	bvar = 0.2;    // subtreshold adaptation
	cvar = -65e-3; // reset voltage
	dvar = 2.0e-3; // spike triggered adaptation
	thr = 30e-3; // spike cutoff (reset threshold)

	// adaptation_vector = get_state_vector("izhi_adaptation");
	// cur_exc = get_state_vector("cur_exc");
	// cur_inh = get_state_vector("cur_inh");
	// temp_vector = get_state_vector("_temp");
}

inline void integrate_state();
inline void check_threshold();

void evolve(){
	float temp_membrane_voltage;
	float temp_adapt_var;
	
	// compute synaptic conductance based currents
    // excitatory
	exc_current = -g_ampa*membrane_voltage;
    
    // inhibitory
	inh_current = g_gaba*(membrane_voltage - e_rev_gaba);

	// compute izhikevich neuronal dynamics
	temp_membrane_voltage = 40 * sqr(membrane_voltage);
	temp_membrane_voltage += (5*membrane_voltage);
	temp_membrane_voltage +=0.140;
///
	temp_membrane_voltage-=adaptation_var;

	// add synaptic currents
	temp_membrane_voltage+=exc_current;
	temp_membrane_voltage-=ihn_current;

	// update membrane voltage
	// TODO define auryn_timestep
	membrane_voltage+= (auryn_timestep/1e-3)*temp_membrane_voltage; // division by 1e-3 due to rescaling of time from ms -> s

	// update adaptation variable
	temp_adapt_var = membrane_voltage;
	temp_adapt_var *= bvar;
	temp_adapt_var -= adaptation_var;
	adaptation_var+=(avar*auryn_timestep/1e-3)*temp_adapt_var;

	// Check possibility to change mode
	check_threshold();

	// decay synaptic conductances
	g_ampa*=scale_ampa;
	g_gaba*=scale_gaba;
}

void setAllPost(float g)

void check_threshold()
{
   	if (membrane_voltage>thr){
		//Updating connections with post-neurons (Sparse connections)
		if(neuronType=="excitatory"){
			setAllPost(g_ampa,0.3);
		}
		else{setAllPost(g_gaba,0.3);}
		
		push_spike(i); //Notify a spike and write the time stamp of the spike in memory
		
		//Update STDP connections
		//Get all presynaptic neurons that spiked within the past 75ms
		//For all presynaptic neurons compute Delta_t = time_now - time_pre_synaptic_spike
		
		//Reset neuron variables and go back to sub-threshold mode
		membrane_voltage = cvar; // reset mem
		adaptation_var+=dvar; // increase adapt variable
	}
}

