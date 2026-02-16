using UnityEngine;

public class Treasure : MonoBehaviour
{
    private bool isBeingCollected = false;
	public AudioClip collectSound; // Ήχος για τη συλλογή
    private AudioSource audioSource; // Για να παίξουμε τον ήχο
	public GameObject collectEffectPrefab; // Αναφορά στο Particle Effect Prefab
	
	void Start()
    {
        audioSource = GetComponent<AudioSource>(); // Λήψη του AudioSource του αντικειμένου
    }

    void OnTriggerEnter(Collider other)
    {
        if (!isBeingCollected && other.CompareTag("Player"))
        {
            isBeingCollected = true;
            StartCoroutine(CollectTreasure());
        }
    }

	private int GetTreasurePoints()
	{
		Renderer renderer = GetComponent<Renderer>();
		if (renderer != null && TreasureManager.Instance.treasureMaterials != null)
		{
			for (int i = 0; i < TreasureManager.Instance.treasureMaterials.Length; i++)
			{
				// Χρησιμοποιούμε ReferenceEquals για να συγκρίνουμε τα υλικά
				if (ReferenceEquals(renderer.sharedMaterial, TreasureManager.Instance.treasureMaterials[i]))
				{
					return TreasureManager.Instance.treasurePoints[i];
				}
			}
		}
		return 0; // Default βαθμολογία αν δεν βρεθεί αντιστοιχία
	}


	private System.Collections.IEnumerator CollectTreasure()
	{
		// Παίξιμο ήχου
		if (audioSource != null && collectSound != null)
		{
			audioSource.PlayOneShot(collectSound);
		}
		
		 // Ενεργοποίηση Particle Effect
		if (collectEffectPrefab != null)
		{
			// Δημιουργία του Particle System στην θέση του θησαυρού
			GameObject particleEffect = Instantiate(collectEffectPrefab, transform.position, Quaternion.identity);

			// Περιμένουμε μέχρι να ολοκληρωθεί το particle effect 
			float effectDuration = 2.0f; // Διάρκεια
			Destroy(particleEffect, effectDuration); // Καταστρέφει το particle effect μετά την διάρκεια
		}

		// Προσθήκη σκορ
		int points = GetTreasurePoints();
		ScoreManager.Instance.AddScore(points);

		float shrinkSpeed = 5f;
		while (transform.localScale.x > 0.1f)
		{
			transform.localScale -= Vector3.one * shrinkSpeed * Time.deltaTime;
			yield return null;
		}
		Destroy(gameObject); // Καταστροφή θησαυρού
		TreasureManager.Instance.TreasureCollected();
	}

}
