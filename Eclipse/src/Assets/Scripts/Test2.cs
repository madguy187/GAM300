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
      //Spot.transform.position = transform.position;

       if (Input.GetKey(KeyCode.W))
        {
          Vector3 temp = transform.position;
          transform.position = transform.position + transform.forward * 0.3f;
        }
        if (Input.GetKey(KeyCode.S))
        {
          Vector3 temp = transform.position;
          transform.position = transform.position - transform.forward * 0.3f;
        }
        if (Input.GetKey(KeyCode.D))
        {
          transform.Rotate(new Vector3(0, 2, 0));
        }
        if (Input.GetKey(KeyCode.A))
        {
          transform.Rotate(new Vector3(0, -2, 0));
        }
		
        if (Input.GetKeyDown(KeyCode.E))
        {
            RaycastHit hit;

            if(iscreated == false)
            {
                if (Physics.Raycast(transform.position, transform.forward, out hit, 20.0f))
                {
                    Spot = Instantiate("", transform.position, transform.forward);
                    light = new Light(Spot.Entity);

                    //Console.WriteLine("test" + transform.forward);

                    Spot.transform.position = transform.position;
                    light.Direction = transform.forward;

					if(light.Intensity <= 10.0f )
					{					
						light.Intensity = light.Intensity + 2.0f;
					}
                    iscreated = true;

                    //Console.WriteLine("test" +  light.Intensity);
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
					light.Intensity = light.Intensity - Time.fixedDeltaTime;
					light.Intensity = light.Intensity - Time.fixedDeltaTime;
					light.Intensity = light.Intensity - Time.fixedDeltaTime;
					light.Intensity = light.Intensity - Time.fixedDeltaTime;
				}
				else
				{
					light.Intensity = 0;
				}
			}
		}
        
    }
}