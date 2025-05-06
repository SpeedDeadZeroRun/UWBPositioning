using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using TMPro;
using UnityEngine;
using UnityEngine.Networking;
using UnityEngine.SceneManagement;
using UnityEngine.UI;
using UnityEngine.UIElements;
using static System.Net.WebRequestMethods;

public class TagController : MonoBehaviour
{
    private string URL = "https://project.nwisaku.xyz/api/GetTagMeasureLogs/7";
    private string token ;

   
    public TextMeshProUGUI [] Distance;
    public TextMeshProUGUI[] Anchor_Position;
    public TextMeshProUGUI PositionTag ;
    public Transform[] anchors;
    private float scale = (float)0.008 ;
    private int t_threshold = 1;
    private int c_threshold = 20;

    // Start is called before the first frame update
    void Start()
    {
        token = StateTokenController.token;
        StartCoroutine(GetData());
    }

    // Update is called once per frame
    void Update()
    {
    }

    IEnumerator GetData()
    {
        
        float[] distance = new float[4];

        while (true)
        {
           
            using (UnityWebRequest request = UnityWebRequest.Get(URL))
            {

                request.SetRequestHeader("Authorization", "Bearer " + token);
                yield return request.SendWebRequest();

                if (request.result != UnityWebRequest.Result.Success)
                {
                    Debug.LogError(request.error);
                    yield break;
                }
                else
                {
                    string json = request.downloadHandler.text;
                    SimpleJSON.JSONNode data = SimpleJSON.JSON.Parse(json);

                    for (int i = 0; i < Distance.Length; i++)
                    {
                        int currentAnchor = data["data"][i]["anchor"];
                        float currentDistance = data["data"][i]["distance"];
                        string distanceText = "(Distance : " + currentDistance + " mm)";
                        int distanceIndex = -1;
                        switch (currentAnchor)
                        {
                            case 2: 
                                distanceIndex = 0;
                                break;
                            case 3: 
                                distanceIndex = 1;
                                break;
                            case 4: 
                                distanceIndex = 2;
                                break;
                            case 5: 
                                distanceIndex = 3;
                                break;
                            default:
                                Debug.LogError("Anchor number does not exist");
                                break;
                        }
                        distance[distanceIndex] = currentDistance;
                        Distance[distanceIndex].text = distanceText ;
                    }
                                    
                }

            }

            // {x, y, z, r} units:milimeter
            List<float[]> position_anchor = new List<float[]> {
                new float[4] { 0, 0, 0, distance[0] },                 // AnchorA : x1,y1,z1,r1
                new float[4] { 1000, 0, 0, distance[1] },              // AnchorB : x2,y2,z2,r2
                new float[4] { 500, 866.031f, 0, distance[2] },        // AnchorC : x3,y3,z3,r3
                new float[4] { 500, 288.677f, 816.494f, distance[3] }  // AnchorD : x4,y4,z4,r4
            };

            for (int i = 0; i < Anchor_Position.Length; i++)
            {
                Anchor_Position[i].text = "x : " + position_anchor[i][0] + "  " + "y : " + position_anchor[i][1] + "  " + "z : " + position_anchor[i][2];
            }

            float[,] position = CalculatePosition(position_anchor[0], position_anchor[1], position_anchor[2], position_anchor[3]);

            //Round to 3 decimal places
            float positionX = Convert.ToSingle(Math.Round(position[0, 0], 3));
            float positionY = Convert.ToSingle(Math.Round(position[1, 0], 3));
            float positionZ = Convert.ToSingle(Math.Round(position[2, 0], 3));

            PositionTag.text = "x : " + positionX + "  mm\n\n" + "y : " + positionY + "  mm\n\n" + "z : " + positionZ + "  mm";

            float unity_originZ = -10;
            transform.position = new Vector3(positionX  * scale, positionZ * scale, unity_originZ - (positionY * scale));

            yield return new WaitForSeconds(1f);


        }
      
    }


    /*
     * Parameter:
     * a1 (array): anchorA position(x1,y1,z1,r1)
     * a2 (array): anchorB position(x2,y2,z2,r2)  
     * a3 (array): anchorC position(x3,y3,z3,r3)  
     * a4 (array): anchorD position(x4,y4,z4,r4)  
     * 
     * Return:
     * 2d-array : the estimate position(x,y,z) from tag refer to all set of anchor
    */
    private float[,] CalculatePosition(float[] a1, float[] a2, float[] a3, float[] a4)
     {

        // All Set of Anchor nC3
        List<List<float[]>> all_set_anchor = new List<List<float[]>> {
            new List<float[]> { a1, a2, a3 }, // Set of Anchor A,B,C
            new List<float[]> { a1, a2, a4 }, // Set of Anchor A,B,D
            new List<float[]> { a1, a3, a4 }, // Set of Anchor A,C,D
            new List<float[]> { a2, a3, a4 }  // Set of Anchor B,C,D
        };

        
        float[,] initial_guess = new float[3, 1] { { 1 }, { 1 }, { 1 } };
        List < float[,]> set_of_position = new List<float[,]>();

        int i = 0;
        foreach(List<float[]> set_anchor in all_set_anchor)
        {
            float[,] inv_matrix = InverseJacobianMatrix3x3(set_anchor, initial_guess);
            float[,] initial_func_matrix = FindFunctionValue(set_anchor, initial_guess);
            set_of_position.Add(NewtonRapsonMethod(inv_matrix, initial_func_matrix, set_anchor, initial_guess));
            initial_guess = (float[,])set_of_position[i].Clone(); //Do chaining method
            if(i == 3) // Chain last init_guess to first init_guess
            {
                set_of_position[0] = NewtonRapsonMethod(inv_matrix, initial_func_matrix, set_anchor, initial_guess);
            }
            i++;
        }

      
        float x_mean = 0 , y_mean = 0, z_mean = 0;
        foreach(float[,] pos in set_of_position)
        {
            x_mean += pos[0, 0] / set_of_position.Count;
            y_mean += pos[1, 0] / set_of_position.Count;
            z_mean += pos[2, 0] / set_of_position.Count;
        }

        float[,] final_result = new float[3, 1] { { x_mean } , { y_mean }, {z_mean } };


        return final_result;
      }

    /*
     * Parameter:
     * inv_matrix (2d-array): inverse of matrix size3*3
     * initial_func_matrix (2d-array): matrix size3*1 with initial guess value
     * set_anchor (List of array): each set of anchor in C(4,3)
     * initial_guess (2d-array): initial guess value of matrix size3*1   
     * 
     * Return:
     * 2d-array : the estimate position(x,y,z) from tag refer to specific set of anchor
    */
    private float[,] NewtonRapsonMethod(float [,] inv_matrix , float[,] initial_func_matrix, List<float[]> set_anchor,float [,] initial_guess)
    {

        float[,] current_round = new float[3, 1];
        float[,] current_matrix = (float[,])inv_matrix.Clone();
        float[,] current_func = (float[,])initial_func_matrix.Clone();
        float[,] guess = (float[,])initial_guess.Clone();
        float[,] multiply_matrix = Matrix3x3_multiplication(inv_matrix, current_func);

        for (int i = 0; i < 7; i++) 
        {
            //Global Newton with 4C3
            float a = 1;
            for (int k = 0; k < c_threshold; k++)
            {
                for (int j = 0; j < 3; j++)
                {
                    current_round[j, 0] = guess[j, 0] - (a * multiply_matrix[j, 0]);
                }
                int fout_is_less_than_fin = Enumerable.Range(0, 3).Sum(i => Math.Abs(FindFunctionValue(set_anchor, current_round)[i, 0]) < Math.Abs(FindFunctionValue(set_anchor,guess)[i, 0]) ? 1 : 0);
                if (fout_is_less_than_fin >= t_threshold)
                {
                    Array.Copy(current_round, guess, guess.Length);
                }
                inv_matrix = InverseJacobianMatrix3x3(set_anchor, guess);
                current_func = FindFunctionValue(set_anchor, guess);
                multiply_matrix = Matrix3x3_multiplication(inv_matrix, current_func);
                if (fout_is_less_than_fin >= t_threshold) break;
                a /= 2;
            }

        }
     
        return guess;
    }

    /*
     * Parameter:
     * set_anchor (List of array) : each set of anchor in C(4,3)
     * initial_guess (2d-array): initial guess value of matrix size3*1 
     * 
     * Return:
     * 2d-array : function value of matrix size3*1
    */
    private float[,] FindFunctionValue(List<float[]> set_anchor,float[,] initial_guess)
    {
        float[,] func_val = new float[3, 1];

        for (int i = 0;i < 3; i++)
        {
            func_val[i, 0] = Convert.ToSingle(Math.Pow(initial_guess[0, 0] - set_anchor.ElementAt(i)[0], 2) + Math.Pow(initial_guess[1, 0] - set_anchor.ElementAt(i)[1], 2) + Math.Pow(initial_guess[2, 0] - set_anchor.ElementAt(i)[2], 2) - Math.Pow(set_anchor.ElementAt(i)[3],2));
        }
       
        return func_val ;
    }

    /*
     * Parameter:
     * inv_matrix (2d-array) : inverse of matrix size3*3 
     * func_matrix (2d-array): function of matrix size3*1 
     * 
     * Return:
     * 2d-array : multiplication of matrix size3*3
    */
    private float[,] Matrix3x3_multiplication(float[,] inv_matrix, float[,] func_matrix)
    {
        float[,] multi_matrix = new float[3, 1];

        for(int i = 0; i < 3; i++)
        {
            multi_matrix[i,0] = inv_matrix[i, 0] * func_matrix[0, 0] + inv_matrix[i, 1] * func_matrix[1, 0] + inv_matrix[i, 2] * func_matrix[2, 0];
        }

        return multi_matrix ;
    }

    /*
     * Parameter:
     * matrix (2d-array) : matrix size3*3 
     * 
     * Return:
     * 2d-array : determinant of matrix size3*3
    */
    private float DeterminantMatrix3x3(float[,] matrix)
    {

        float det = 0;
        for(int i = 0;i < 3; i++)
        {
            det += (matrix[0, i] * matrix[1, (i + 1) % 3] * matrix[2, (i + 2) % 3] - matrix[2,i] * matrix[1,(i + 1) % 3] * matrix[0,(i + 2) % 3]);
        }
     
        return det ;
    }

   /*
    * Parameter:
    * matrix (2d-array) : matrix size2*2 
    * 
    * Return:
    * 2d-array : determinant of matrix size2*2
   */
    private float DeterminantMatrix2x2(float[,] matrix)
    {
        float det = matrix[0, 0] * matrix[1, 1] - matrix[1, 0] * matrix[0,1];
        
        return det;
    }

    /*
     * Parameter:
     * matrix (2d-array) : matrix size3*3 
     * row (int) : remove specific row of matrix size3*3
     * col (int) : remove specific column of matrix size3*3
     * 
     * Return:
     * 2d-array : submatrix size2*2
    */
    private float[,] SubMatrix3x3(float[,] matrix , int row , int col)
    {

        float[,] submatrix = new float[2, 2];
        for(int i = 0;i < 3; i++)
        {
            for(int j = 0;i != row && j < 3; j++)
            {
                if(j != col)
                {
                    submatrix[i < row ? i : i - 1,j < col ? j : j-1] = matrix[i,j];
                }
            }
        } 

        return submatrix;
    }

    /*
     * Parameter:
     * matrix (2d-array) : matrix size3*3 
     * 
     * Return:
     * 2d-array : transpose of matrix size3*3
    */
    private float[,] TransposeMatrix3x3(float[,] matrix)
    {
        float[,] trans_matrix = new float[3, 3];

        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                trans_matrix[j, i] = matrix[i, j];
            }
        }

        return trans_matrix;
    }


    /*
     * Parameter:
     * input (List of array) :  all possible set of anchor in C(4,3) 
     * initial_guess (2d-array) : initial guess value of matrix size3*1
     * 
     * Return:
     * 2d-array : inverse of partial_matrix size3*3
    */
    private float[,] InverseJacobianMatrix3x3(List<float[]> input, float[,] initial_guess)
    {
        float[,] jaco_matrix = new float[3, 3];

        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                jaco_matrix[i, j] = 2 * (initial_guess[j,0] - input.ElementAt(i)[j]) ;
            }
            
        }

        float[,] trans_matrix = TransposeMatrix3x3(jaco_matrix);
        float[,] inv_jaco_matrix = InverseMatrix3x3(trans_matrix);


        return inv_jaco_matrix;
    }

    /*
     * Parameter:
     * t_matrix (2d-array) : transpose of matrix size3*3
     * 
     * Return:
     * 2d-array : inverse of matrix size3*3
    */
    private float[,] InverseMatrix3x3(float[,] t_matrix)
    {
        float[,] inv = new float[3, 3];

        float det = DeterminantMatrix3x3(t_matrix);
        
       
        if (Math.Abs(det) == 0)
        {
            throw new InvalidOperationException("Matrix is not invertible.");
        }
        

        //cofactor matrix2*2 for each row and column
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                inv[i, j] = Convert.ToSingle(Math.Pow(-1, i + j)) * DeterminantMatrix2x2(SubMatrix3x3(t_matrix,i,j)) / det;
            }
        }

        return inv;
    }

    

}



