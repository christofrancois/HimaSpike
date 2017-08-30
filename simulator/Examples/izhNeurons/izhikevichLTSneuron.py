from brian2 import *

num_neurons = 1
duration = 300*ms

# Izhikevich Parameters
C = 100*pF
k = 1*pA/mV/mV
vr = -56*mV
vt = -42*mV
a = 0.03*kHz
b = 8*pA/mV
c = -53*mV
d = 20*pA
vpeak = 40*mV

# The Izhikevich model
eqs = '''
dv/dt=(k*(v-vr)*(v-vt)-u+I)/C : volt 
du/dt=a*(b*(v-vr)-u) : amp 
I : amp
'''

reset = '''
v=c+(0.04*Gohm*u)
u = (u+d)*int(u+d <= 670*pA) + 670*pA *int(u+d > 670*pA)
'''

neuron = NeuronGroup(num_neurons, model=eqs, reset=reset, threshold='v > (vpeak-(0.1*Gohm*u))')
neuron.v = vr
neuron.I = 200 * pA

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