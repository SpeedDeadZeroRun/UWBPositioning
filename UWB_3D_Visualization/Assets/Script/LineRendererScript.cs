using System.Collections;
using System.Collections.Generic;
using UnityEngine;


public class LineRendererScript : MonoBehaviour
{

    public LineRenderer line;
    public Transform anchor_pos;
    public Transform tag_pos;

    // Start is called before the first frame update
    void Start()
    {
        line.positionCount = 2;
    }

    // Update is called once per frame
    void Update()
    {
        line.SetPosition(0,tag_pos.position);
        line.SetPosition(1,anchor_pos.position); 
    }
}
