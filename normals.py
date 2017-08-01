
# coding: utf-8

# In[63]:

import math
def normals(x1,y1,z1,x2,y2,z2,x3,y3,z3):
    u=[x2-x1,y2-y1,z2-z1]
    v=[x3-x1,y3-y1,z3-z1]
    print u,v
    nx=(u[1]*v[2])-(u[2]*v[1])
    ny=(u[2]*v[0])-(u[0]*v[2])
    nz=(u[0]*v[1])-(u[1]*v[0])
    
    return nx,ny,nz

    
    


# In[68]:

i,j,k=normals(0, 0, 0,0, 0.5, 0,-0.25, 0.25, 0.1)
print i,j,k


# In[69]:


n=math.sqrt(i*i+j*j+k*k);
print i/n, j/n,k/n


# In[ ]:




# In[ ]:



