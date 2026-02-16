using UnityEngine;

public class CameraController : MonoBehaviour
{
    public float moveSpeed = 10f; // Ταχύτητα κίνησης της κάμερας
    public float rotateSpeed = 100f; // Ταχύτητα περιστροφής της κάμερας

    void Update()
    {
        // Κίνηση στους άξονες x και z με τα βελάκια
        float moveX = Input.GetAxis("Horizontal") * moveSpeed * Time.deltaTime;
        float moveZ = Input.GetAxis("Vertical") * moveSpeed * Time.deltaTime;

        // Κίνηση στον άξονα y με τα πλήκτρα + και -
        float moveY = 0f;
        if (Input.GetKey(KeyCode.KeypadPlus) || Input.GetKey(KeyCode.PageUp))
        {
            moveY = moveSpeed * Time.deltaTime;
        }
        else if (Input.GetKey(KeyCode.KeypadMinus) || Input.GetKey(KeyCode.PageDown))
        {
            moveY = -moveSpeed * Time.deltaTime;
        }

        // Εφαρμογή κίνησης
        transform.position += new Vector3(moveX, moveY, moveZ);

        // Περιστροφή (clockwise) γύρω από τον άξονα y με το πλήκτρο R
        if (Input.GetKey(KeyCode.R))
        {
            float rotationY = rotateSpeed * Time.deltaTime;
            transform.Rotate(0, rotationY, 0, Space.Self);
        }
        // Περιστροφή (counterclockwise) γύρω από τον άξονα y με το πλήκτρο E
        if (Input.GetKey(KeyCode.E))
        {
            float rotationY = rotateSpeed * Time.deltaTime;
            transform.Rotate(0, -rotationY, 0, Space.Self);
        }
    }
}
