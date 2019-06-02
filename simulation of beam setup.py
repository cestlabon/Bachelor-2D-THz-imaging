#!/usr/bin/env python
# coding: utf-8

# In[60]:


get_ipython().run_line_magic('matplotlib', 'inline')
import matplotlib.pyplot as plt
import numpy as np
import poppy
from poppy.poppy_core import PlaneType
import astropy.units as u
import astropy.io.fits as fits
from scipy.constants import c


# In[61]:


v = 1e12
wl = c/v
npix = 1024 # ~512 is minimum to accurately recover the central diffraction spike
wf = poppy.FresnelWavefront(6.2*u.mm,wavelength=wl,npix=npix,oversample=4)
wf *= poppy.CircularAperture(radius=6.2*u.mm)


# Emitter

# In[62]:


conv_lensa = poppy.QuadraticLens(22*u.mm)
wf *= conv_lensa
#plt.figure(figsize=(12,5))
#wf.display('both',colorbar=True, showpadding=True)
#plt.suptitle("Entrance Pupil", fontsize=18)
#print("After aperture, beam waist is at {:.2f}".format(wf.z_w0) )


# In[63]:


x = 22*u.mm
wf.propagate_fresnel(x)
wf.spot_radius(z=None)
#wf*= poppy.CircularAperture(radius=6.2*u.m)
#plt.figure(figsize=(12,5))
#wf.display('both',colorbar=True, showpadding=True)
#plt.suptitle("cirkel", fontsize=18)


# In[64]:


Gaus = poppy.GaussianAperture(fwhm=1.2*u.mm)
wf *= Gaus


# In[65]:


x = 22*u.mm
wf.propagate_fresnel(x)
wf.spot_radius(z=None)


# In[66]:


conv_lensb = poppy.QuadraticLens(22*u.mm)
wf *= conv_lensb


# In[67]:


x = 11*u.mm
wf.propagate_fresnel(x)
wf.spot_radius(z=None)


# In[68]:


#Gaus = poppy.GaussianAperture(fwhm=8.9*u.mm)
#wf *= Gaus


# In[69]:


x = 11*u.mm
wf.propagate_fresnel(x)
wf.spot_radius(z=None)


# In[ ]:





# In[70]:


conc_lens = poppy.QuadraticLens(-22*u.mm)
wf *= conc_lens
print("After lens with focal length {}, beam waist is at {:.2f}".format(conc_lens.fl, wf.z_w0) )


# In[71]:


wf.spot_radius(z=None)


# In[72]:


plt.figure(figsize=(12,5))
wf.display('both',colorbar=True, showpadding=True)
plt.suptitle("efter emitter", fontsize=18)


# Beam after emitter

# In[73]:


x = 44*u.mm
wf.propagate_fresnel(x)
print("After prop, beam waist is at {:.2f}".format(wf.z_w0) )
#plt.figure(figsize=(12,5))
#wf.display('both',colorbar=True, showpadding=True)
#plt.suptitle("Starting beam", fontsize=18)


# In[74]:


wf.spot_radius(z=None)


# In[75]:


#wf.r_c(z=None)


# In[76]:


plt.figure(figsize=(12,5))
wf.display('both',colorbar=True, showpadding=True)
plt.suptitle("inden første parabol", fontsize=18)


# Parabola culliminating the beam

# In[77]:


conv_lens = poppy.QuadraticLens(66*u.mm)
wf *= conv_lens
parabol1 = poppy.SquareAperture(size=50*u.mm)
wf *= parabol1


# In[78]:


#Gaus2 = poppy.GaussianAperture(fwhm=8.9*u.mm)
#wf *= Gaus2


# Checking if the is in fact culliminated

# In[79]:


x = 20*u.mm
wf.propagate_fresnel(x)
#print("After prop, beam waist is at {:.2f}".format(wf.z_w0) )
plt.figure(figsize=(12,5))
wf.display('both',colorbar=True, showpadding=True)
plt.suptitle("collimineret stråle", fontsize=18)


# In[80]:


wf.spot_radius(z=None)


# In[81]:


x = 80*u.mm
wf.propagate_fresnel(x)
wf.spot_radius(z=None)


# Parabola focusing the beam

# In[82]:


conv_lens2 = poppy.QuadraticLens(66*u.mm)
wf *= conv_lens2
parabol2 = poppy.SquareAperture(size=50*u.mm)
wf *= parabol2


# In[83]:


x = 66*u.mm
wf.propagate_fresnel(x)
#print("After prop, beam waist is at {:.2f}".format(wf.z_w0) )
#plt.figure(figsize=(12,5))
#wf.display('both',colorbar=True, showpadding=True)
#plt.suptitle("facol punkt", fontsize=18)


# In[84]:


#Gaus3 = poppy.GaussianAperture(fwhm=1.2*u.mm)
#wf *= Gaus3


# In[85]:


#Gaus2 = poppy.GaussianAperture(fwhm=1.2*u.mm)
#wf *= Gaus2


# In[86]:


wf.spot_radius(z=None)


# In[87]:


plt.figure(figsize=(12,5))
wf.display('both',colorbar=True, showpadding=True)
plt.suptitle("focus pkt", fontsize=18)


# In[ ]:





# In[ ]:





# In[ ]:





# In[ ]:





# In[ ]:





# In[ ]:





# In[ ]:





# In[ ]:





# In[ ]:




