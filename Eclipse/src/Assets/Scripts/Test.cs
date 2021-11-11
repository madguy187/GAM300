using System;
using System.Runtime.CompilerServices;
using System.Collections.Generic;
using Eclipse;

public class Test : EclipseBehavior
{
    public Light light;
    public float temp;
	
	[Header("Object Interactions")]
    public GameObject touchedObject;
    public GameObject pickedObject;
	
    public void Start()
    {
    }

    public void Update()
    {
        Console.WriteLine(light.Entity);
        Console.WriteLine(temp);
    }
}