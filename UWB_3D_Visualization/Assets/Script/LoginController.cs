using System.Collections;
using System.Collections.Generic;
using UnityEngine.Networking;
using UnityEngine.SceneManagement;
using UnityEngine;
using TMPro;
using UnityEngine.UI;

public class LoginController : MonoBehaviour
{

    public TMP_InputField accountUserName;
    public TMP_InputField accountPassWord;
    public TextMeshProUGUI UsernameErrorText;
    public TextMeshProUGUI PasswordErrorText;
    public TextMeshProUGUI LoginErrorText;
    public Image ErrorBg;


    // Start is called before the first frame update
    void Start()
    {
 
    }

    // Update is called once per frame
    void Update()
    {

    }

    public void AccountLogin()
    {
        string userName = accountUserName.text;
        string passWord = accountPassWord.text;
        StartCoroutine(PostData(userName, passWord));
    }

    IEnumerator PostData(string userName, string passWord)
    {


        // Reset error messages
        UsernameErrorText.text = "";
        PasswordErrorText.text = "";

        // Check for required fields
        if (string.IsNullOrEmpty(userName))
        {
            UsernameErrorText.text = "Username is a required field.";
        }

        if (string.IsNullOrEmpty(passWord))
        {
            PasswordErrorText.text = "Password is a required field.";
        }
     
        // Stop execution if there are errors
        if (!string.IsNullOrEmpty(UsernameErrorText.text) || !string.IsNullOrEmpty(PasswordErrorText.text))        
        {
            yield break;
        }

     
        WWWForm form = new WWWForm();
        form.AddField("name", userName);
        form.AddField("secret", passWord);
        using (UnityWebRequest request = UnityWebRequest.Post($"https://project.nwisaku.xyz/api/login?name={userName}&secret={passWord}", form))
        {
            yield return request.SendWebRequest();
            if (request.result != UnityWebRequest.Result.Success)
            {
                if(request.responseCode == 404)
                {
                    LoginErrorText.text = "Incorrect username or password.";
                }
                else
                {
                    LoginErrorText.text = "Fail to communicate with the server.";
                }
                ErrorBg.GetComponent<Image>().color = new Color32(255, 215, 210, 255); //Red Color Message
                yield break;
            }
            else
            {
                Debug.Log("Login Successfull!");
                string json = request.downloadHandler.text;
                SimpleJSON.JSONNode data = SimpleJSON.JSON.Parse(json);
                StateTokenController.token = data["token"];
                SceneManager.LoadScene("Simulation");
            }
        }
 

    }

}
