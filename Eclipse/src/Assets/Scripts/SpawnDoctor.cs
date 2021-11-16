using System;
using System.Runtime.CompilerServices;
using System.Collections.Generic;
using Eclipse;

public class SpawnDoctor : EclipseBehavior
{
    private GameObject obj;
    bool isCreated = false;

    public void OnCollision()
    {
      if (!isCreated)
      {
        Vector3 temp = transform.position;
        temp.x += 5.0f;
        temp.y -= 2.5f;
        Vector3 rot = new Vector3(0.0f, 180.0f, 0.0f);
        obj = CreatePrefab("DoctorIslami", temp, rot);
        isCreated = true;
      }
    }

    public void Start()
    {
    }

    public void FixedUpdate()
    {
        if (obj != null)
        {
          Vector3 temp = obj.transform.position;
          temp.z -= 0.05f;
          obj.transform.position = temp;
        }
    }
}