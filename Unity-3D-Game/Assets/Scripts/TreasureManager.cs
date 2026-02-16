using UnityEngine;

public class TreasureManager : MonoBehaviour
{
    public static TreasureManager Instance { get; private set; }

    public GameObject treasurePrefab; // Το prefab του θησαυρού
    public Material[] treasureMaterials; // Υλικά θησαυρών
	public int[] treasurePoints; // Βαθμολογία ανά υλικό
    public float treasureLifetime = 5f; // Διάρκεια ζωής του θησαυρού
    public Vector3 mazeBounds = new Vector3(45, 0, 45); // Όρια λαβύρινθου
    public GameObject treasureBob; // Αναφορά στον Treasure Bob

    private GameObject currentTreasure;

    void Awake()
    {
        if (Instance == null)
        {
            Instance = this;
        }
        else
        {
            Destroy(gameObject);
        }
    }

    void Start()
    {
        SpawnTreasure();
    }

    void SpawnTreasure()
    {
        if (currentTreasure != null)
        {
            Destroy(currentTreasure); // Καταστροφή παλιού θησαυρού
        }

        Vector3 randomPosition;
        int attempts = 0;
        bool isValidPosition = false;
        float minDistanceFromBob = 10f; // Ελάχιστη απόσταση από τον Treasure Bob

        do
        {
            // Επιλογή τυχαίας θέσης
            randomPosition = new Vector3(
                Random.Range(-mazeBounds.x, mazeBounds.x),
                2.5f,
                Random.Range(-mazeBounds.z, mazeBounds.z)
            );

            // Έλεγχος αν η θέση βρίσκεται μέσα σε τοίχο
            bool notInWall = !Physics.CheckSphere(randomPosition, 2.5f, LayerMask.GetMask("Wall"));

            // Έλεγχος απόστασης από τον Bob
            bool farFromBob = Vector3.Distance(randomPosition, treasureBob.transform.position) > minDistanceFromBob;

            isValidPosition = notInWall && farFromBob;
            attempts++;
        }
        while (!isValidPosition && attempts < 100); // Περιορισμός προσπαθειών για αποφυγή infinite loop

        if (isValidPosition)
        {
            // Δημιουργία νέου θησαυρού
            currentTreasure = Instantiate(treasurePrefab, randomPosition, Quaternion.identity);
            currentTreasure.GetComponent<Renderer>().material = treasureMaterials[Random.Range(0, treasureMaterials.Length)];

            // Καταστροφή θησαυρού μετά από συγκεκριμένο χρόνο
            Invoke(nameof(CheckAndRespawnTreasure), treasureLifetime);
        }
        else
        {
            Debug.LogWarning("Failed to find a valid position for the treasure.");
        }
    }

    public void TreasureCollected()
    {
        CancelInvoke(nameof(CheckAndRespawnTreasure)); // Ακύρωση αυτόματου χρόνου αν μαζευτεί
        SpawnTreasure();
    }

    private void CheckAndRespawnTreasure()
    {
        if (currentTreasure != null) Destroy(currentTreasure);
        SpawnTreasure();
    }
}
