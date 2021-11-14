using System;
using System.Runtime.CompilerServices;
using System.Collections.Generic;
using Eclipse;

public class Test2 : EclipseBehavior
{
    bool iscreated = false;

    private GameObject Spot;
    private Light light;

    public void Start()
    {
        
    }

    public void Update()
    {	
        if (Input.GetKeyDown(KeyCode.C))
        {
            RaycastHit hit;

            if(iscreated == false)
            {
                if (Physics.Raycast(transform.position, transform.forward, out hit, 20.0f))
                {
                    Spot = CreateSpotLight(transform.position, transform.forward);
                    light = new Light(Spot.Entity);
					
                    Spot.transform.position = hit.point;
					
                    light.Direction = transform.forward;

					if(light.Intensity <= 10.0f )
					{					
						light.Intensity = light.Intensity + 2.0f;
					}
                    iscreated = true;
                }
            }
            else
            {
                    Spot.transform.position = transform.position;
                    light.Direction = transform.forward;
					
					if(light.Intensity <= 10.0f )
					{							
						light.Intensity = light.Intensity + 2.0f;
					}
            }   
        }
		else
		{			
			if(Spot != null)
			{
				if(light.Intensity > 0.0f )
				{
					light.Intensity = light.Intensity - (Time.fixedDeltaTime * 2);
				}
				else
				{
					light.Intensity = 0;
				}
			}
		}
        
    }
}