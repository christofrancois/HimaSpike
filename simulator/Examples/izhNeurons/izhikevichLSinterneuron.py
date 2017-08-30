from brian2 import *

num_neurons = 1
duration = 1.1*second

# Izhikevich Parameters
C = 20*pF
k = 0.3*pA/mV/mV
l = 1.2*pA/mV
vr = -66*mV
vt = -40*mV
a_d = 0.01*kHz
a = 0.17*kHz
b = 5*pA/mV
c = -45*mV
d = 100*pA
vpeak = 30*mV

# The Izhikevich model
eqs = '''
dv/dt=((k*(v-vr)*(v-vt))+(l*(v_d-v))-u+I)/C : volt 
dv_d/dt=a_d*(v-v_d) : volt
du/dt=a*(b*(v-vr)-u) : amp
I : amp
'''

reset = '''
v=c
u+=d
'''

neuron = NeuronGroup(num_neurons, model=eqs, reset=reset, threshold='v > vpeak')

# Monitoring membrane potential
v_mon = StateMonitor(neuron, ('v', 'u'), record=True)
# Monitoring spikes
s_mon = SpikeMonitor(neuron)

neuron.v = vr
neuron.I = 0 * pA
run(500*ms)
neuron.I = 140 * pA
run(500*ms)
neuron.I = 0 * pA
run(duration - 1000*ms, report='text')

# subplot(211)
# Plot membrane potential v
plot(v_mon.t/ms, v_mon.v[0]/mV, label='v')
xlabel('Time (ms)')
ylabel('Voltage (mV)')

# subplot(212)
# plot(v_mon.v[0]/mV, v_mon.u[0]/pA, label='u')
# xlabel('membrane potential v (mV)')
# ylabel('recovery current u (pA)')

# Plot spike counts
# plot(s_mon.t/ms, s_mon.i, '.')
# xlabel('Time (ms)')
# ylabel('Spikes')
tight_layout()
show()