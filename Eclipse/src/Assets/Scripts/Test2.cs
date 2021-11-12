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
                    light.Intensity = 2.0f;
                    iscreated = true;

                    //Console.WriteLine("test" +  light.Intensity);
                }
            }
            else
            {
                if (Physics.Raycast(transform.position, transform.forward, out hit, 5.0f))
                {
                    Spot.transform.position = transform.position;
                    light.Direction = transform.forward;
                    light.Intensity = 2.0f;
                }
            }   
        }
        
    }
}