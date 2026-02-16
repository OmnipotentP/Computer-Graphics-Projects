using UnityEngine;

public class TrapManager : MonoBehaviour
{
    public GameObject trapPrefab; // Το prefab της παγίδας
    public Vector3 mazeBounds; // Όρια του λαβύρινθου (π.χ., 50 x 50 για επίπεδο 100x100)
    public float minSpawnTime = 5f; // Ελάχιστος χρόνος για εμφάνιση παγίδας
    public float maxSpawnTime = 15f; // Μέγιστος χρόνος για εμφάνιση παγίδας
    public float minDuration = 5f; // Ελάχιστη διάρκεια ζωής παγίδας
    public float maxDuration = 10f; // Μέγιστη διάρκεια ζωής παγίδας
    public GameObject treasureBob; // Αναφορά στον Treasure Bob

    private GameObject currentTrap; // Η τρέχουσα παγίδα

    void Start()
    {
        ScheduleNextTrap(); // Προγραμματίζουμε την πρώτη παγίδα
    }

    void ScheduleNextTrap()
    {
        float spawnTime = Random.Range(minSpawnTime, maxSpawnTime);
        Invoke(nameof(SpawnTrap), spawnTime);
    }

    void SpawnTrap()
    {
        if (currentTrap != null)
        {
            Destroy(currentTrap); // Καταστρέφουμε την προηγούμενη παγίδα αν υπάρχει
        }

        Vector3 randomPosition;
        int attempts = 0;
        bool isValidPosition = false;
        float minDistanceFromBob = 7f; // Ελάχιστη απόσταση από τον Treasure Bob

        do
        {
            // Επιλέγουμε τυχαία θέση
            randomPosition = new Vector3(
                Random.Range(-mazeBounds.x, mazeBounds.x),
                2.5f, // Ύψος της παγίδας
                Random.Range(-mazeBounds.z, mazeBounds.z)
            );

            // Έλεγχος αν η θέση βρίσκεται μέσα σε τοίχο
            bool notInWall = !Physics.CheckSphere(randomPosition, 2.5f, LayerMask.GetMask("Wall"));

            // Έλεγχος απόστασης από τον Bob
            bool farFromBob = Vector3.Distance(randomPosition, treasureBob.transform.position) > minDistanceFromBob;

            isValidPosition = notInWall && farFromBob;
            attempts++;
        }
        while (!isValidPosition && attempts < 100);

        if (isValidPosition)
        {
            // Δημιουργούμε την παγίδα
            currentTrap = Instantiate(trapPrefab, randomPosition, Quaternion.identity);

            // Προγραμματίζουμε την καταστροφή της μετά από τυχαία διάρκεια
            float duration = Random.Range(minDuration, maxDuration);
            Destroy(currentTrap, duration);

            // Προγραμματίζουμε την επόμενη παγίδα
            ScheduleNextTrap();
        }
        else
        {
            Debug.LogWarning("Failed to find a valid position for the trap.");
        }
    }
}
