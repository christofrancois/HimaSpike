from brian2 import *

num_neurons = 1
duration = 1*second

# Izhikevich Parameters
C = 150*pF
k = 1.2*pA/mV/mV
vr = -75*mV
vt = -45*mV
a = 0.01*kHz
b = 5*pA/mV
c = -56*mV
d = 130*pA
vpeak = 50*mV

# The Izhikevich model
eqs = '''
dv/dt=(k*(v-vr)*(v-vt)-u+I)/C : volt 
du/dt=a*(b*(v-vr)-u) : amp 
I : amp
'''

reset = '''
v=c
u+=d
'''

neuron = NeuronGroup(num_neurons, model=eqs, reset=reset, threshold='v > vpeak')
neuron.v = vr
neuron.I = 550 * pA

# Monitoring membrane potential
v_mon = StateMonitor(neuron, ('v', 'u'), record=True)
# Monitoring spikes
s_mon = SpikeMonitor(neuron)

run(duration, report='text')

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