using System.Collections;
using System.Collections.Generic;
using Unity.VisualScripting;
using UnityEngine;

public class CameraSystem : MonoBehaviour
{

    private bool useEdgeScrolling = false;
    private bool dragPanMoveActive;
    private Vector3 lastMousePosition;

    // Start is called before the first frame update
    void Start()
    {
       
    }

    // Update is called once per frame
    void Update()
    {
        ActivateEdgeScrolling();
        if (useEdgeScrolling)
        {
            HandleCameraMovementEdgeScrolling();
        }
        HandleCameraMovement();
        HandleCameraMovementDragPan();
        HandleCameraRotation();
        HandleCameraZoom();
    }

    private void HandleCameraMovement()
    {
        Vector3 inputDir = new Vector3(0, 0, 0);
        if (Input.GetKey(KeyCode.A)) inputDir.x = -1f;
        if (Input.GetKey(KeyCode.D)) inputDir.x = +1f;
        if (Input.GetKey(KeyCode.W)) inputDir.y = +1f;
        if (Input.GetKey(KeyCode.S)) inputDir.y = -1f;


        Vector3 moveDir = transform.forward * inputDir.z + transform.right * inputDir.x + transform.up * inputDir.y; ;

        float moveSpeed = 30f;
        transform.position += moveDir * moveSpeed * Time.deltaTime;
    }

    private void HandleCameraMovementEdgeScrolling()
    {
        Vector3 inputDir = new Vector3(0, 0, 0);
     
        int edgeScollSize = 20;
        if (Input.mousePosition.x < edgeScollSize) inputDir.x = -1f;
        if (Input.mousePosition.y < edgeScollSize) inputDir.z = -1f;
        if (Input.mousePosition.x > Screen.width - edgeScollSize) inputDir.x = +1f;
        if (Input.mousePosition.y > Screen.height - edgeScollSize) inputDir.z = +1f;
        

        Vector3 moveDir = transform.forward * inputDir.z + transform.right * inputDir.x;

        float moveSpeed = 30f;
        transform.position += moveDir * moveSpeed * Time.deltaTime;
    }

    private void HandleCameraMovementDragPan()
    {
        Vector3 inputDir = new Vector3(0, 0, 0);

        if (Input.GetMouseButtonDown(0))
        {
            dragPanMoveActive = true;
            lastMousePosition = Input.mousePosition;
        }
        if (Input.GetMouseButtonUp(0))
        {
            dragPanMoveActive = false;
        }
        if (dragPanMoveActive)
        {
            Vector3 mouseMovementDelta = (Vector3)Input.mousePosition - lastMousePosition;

            float dragPanSpeed = 0.5f;
            inputDir.x = mouseMovementDelta.x * dragPanSpeed;
            inputDir.z = mouseMovementDelta.y * dragPanSpeed;


            lastMousePosition = Input.mousePosition;
        }

        Vector3 moveDir = transform.forward * inputDir.z + transform.right * inputDir.x;

        float moveSpeed = 30f;
        transform.position += moveDir * moveSpeed * Time.deltaTime;
    }

    private void HandleCameraRotation()
    {
        float rotateDir = 0f;
        if (Input.GetKey(KeyCode.Q)) rotateDir = +1f;
        if (Input.GetKey(KeyCode.E)) rotateDir = -1f;

        float rotateSpeed = 200f;
        transform.eulerAngles += new Vector3(0, rotateDir * rotateSpeed * Time.deltaTime, 0);
    }
    private void HandleCameraZoom()
    {
    
        Vector3 inputDir = new Vector3(0, 0, 0);
        if (Input.GetAxis("Mouse ScrollWheel") > 0f) inputDir.z = +10f;
        if (Input.GetAxis("Mouse ScrollWheel") < 0f) inputDir.z = -10f;

        Vector3 moveDir = transform.forward * inputDir.z;

        float moveSpeed = 30f;
        transform.position += moveDir * moveSpeed * Time.deltaTime;
    }


    private void ActivateEdgeScrolling()
    {
        if (Input.GetKeyDown(KeyCode.Space))
        {
            useEdgeScrolling = !useEdgeScrolling;
        }
    }
}
