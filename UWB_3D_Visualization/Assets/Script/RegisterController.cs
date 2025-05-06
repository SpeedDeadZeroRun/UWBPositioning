using System.Collections;
using System.Collections.Generic;
using TMPro;
using UnityEngine;
using UnityEngine.Networking;
using UnityEngine.SceneManagement;

public class RegisterController : MonoBehaviour
{

    public TMP_InputField accountUserName;
    public TMP_InputField accountPassWord;
    public TMP_InputField accountConfirmPassWord;
    public TextMeshProUGUI UsernameErrorText;
    public TextMeshProUGUI PasswordErrorText;
    public TextMeshProUGUI ConfirmpasswordErrorText;


    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        
    }

    public void AccountRegister()
    {
        string userName = accountUserName.text;
        string passWord = accountPassWord.text;
        string confirmpassWord = accountConfirmPassWord.text;
        StartCoroutine(PostData(userName, passWord,confirmpassWord));

    }

    IEnumerator PostData(string userName,string passWord,string confirmpassWord)
    {


        // Reset error messages
        UsernameErrorText.text = "";
        PasswordErrorText.text = "";
        ConfirmpasswordErrorText.text = "";

        // Check for required fields
        if (string.IsNullOrEmpty(userName))
        {
            UsernameErrorText.text = "Username is a required field.";
        }

        if (string.IsNullOrEmpty(passWord))
        {
            PasswordErrorText.text = "Password is a required field.";
        }
        else if (passWord.Length < 8)
        {
            PasswordErrorText.text = "Your Password must be at least 8 characters.";
        }

        if (string.IsNullOrEmpty(confirmpassWord))
        {
            ConfirmpasswordErrorText.text = "Confirm Password is a required field.";
        }
        else if (confirmpassWord != passWord)
        {
            ConfirmpasswordErrorText.text = "Confirm Password and Password does not match.";
        }

        // Stop execution if there are errors
        if (!string.IsNullOrEmpty(UsernameErrorText.text) ||
            !string.IsNullOrEmpty(PasswordErrorText.text) ||
            !string.IsNullOrEmpty(ConfirmpasswordErrorText.text))
        {
            yield break;
        }

        WWWForm form = new WWWForm();
        form.AddField("name", userName);
        form.AddField("secret", passWord);
        using (UnityWebRequest request = UnityWebRequest.Post($"https://project.nwisaku.xyz/api/register?name={userName}&secret={passWord}", form))
        {
            yield return request.SendWebRequest();
            if(request.result != UnityWebRequest.Result.Success)
            {
                Debug.Log(request.error);
                yield break;
            }
            else
            {
                Debug.Log("Successfully Created");
                SceneManager.LoadScene("Login");
            }
        }
    }

}
