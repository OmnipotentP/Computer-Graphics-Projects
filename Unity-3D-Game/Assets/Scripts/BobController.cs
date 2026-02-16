using UnityEngine;

public class BobController : MonoBehaviour
{
    public float[] speedLevels = { 4f, 8f, 16f, 32f, 64f }; // Πέντε διαβαθμίσεις ταχύτητας
    private int currentSpeedIndex = 2; // Ξεκινάμε από τη μέση ταχύτητα
    public LayerMask wallLayer; // Για ανίχνευση τοίχων

    // Όρια του plane
    private float minX = -50f;
    private float maxX = 50f;

    void Update()
    {
        // Αυξομείωση ταχύτητας με τα πλήκτρα O και U
        if (Input.GetKeyDown(KeyCode.O)) // Αύξηση ταχύτητας
        {
            if (currentSpeedIndex < speedLevels.Length - 1)
                currentSpeedIndex++;
        }
        if (Input.GetKeyDown(KeyCode.U)) // Μείωση ταχύτητας
        {
            if (currentSpeedIndex > 0)
                currentSpeedIndex--;
        }

        // Λήψη εισόδου από τον χρήστη
        float moveX = 0f;
        float moveZ = 0f;

        if (Input.GetKey(KeyCode.J)) moveX = -1f;
        if (Input.GetKey(KeyCode.L)) moveX = 1f;
        if (Input.GetKey(KeyCode.I)) moveZ = 1f;
        if (Input.GetKey(KeyCode.K)) moveZ = -1f;

        // Υπολογισμός νέας θέσης
        Vector3 moveDirection = new Vector3(moveX, 0, moveZ).normalized;
        Vector3 newPosition = transform.position + moveDirection * speedLevels[currentSpeedIndex] * Time.deltaTime;

        // Περιορισμός της θέσης εντός του plane
        newPosition.x = Mathf.Clamp(newPosition.x, minX, maxX);
        
        // Έλεγχος για τοίχους
        if (!Physics.CheckSphere(newPosition, 3.5f, wallLayer))
        {
            transform.position = newPosition;
        }
    }

    // Συνάρτηση για να εμφανίσουμε την τρέχουσα ταχύτητα
    void OnGUI()
    {
        GUI.Label(new Rect(10, 10, 200, 20), "Speed: " + speedLevels[currentSpeedIndex]);
    }
}
