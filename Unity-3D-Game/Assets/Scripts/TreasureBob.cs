using UnityEngine;
using System.Collections;

public class TreasureBob : MonoBehaviour
{
    private bool isDead = false; // Κατάσταση του Bob
    public GameObject gameOverText; // Αναφορά στο Game Over Text
    public AudioClip deathSound; // Ήχος για τη συλλογή
    private AudioSource audioSource; // Για να παίξουμε τον ήχο
    public float delayBeforeQuit = 2.0f; // Χρόνος αναμονής πριν τον τερματισμό

    void Start()
    {
        audioSource = GetComponent<AudioSource>(); // Λήψη του AudioSource του αντικειμένου
        if (audioSource != null)
        {
            audioSource.ignoreListenerPause = true; // Αγνόηση "παγώματος" χρόνου
        }
    }
	
	void Update()
    {
        // Έλεγχος αν πατήθηκε το πλήκτρο ESC
        if (Input.GetKeyDown(KeyCode.Escape))
        {
            QuitGame();
        }
    }

    private void OnTriggerEnter(Collider other)
    {
        if (other.gameObject.layer == LayerMask.NameToLayer("Trap") && !isDead)
        {
            // Παίξιμο ήχου
            if (audioSource != null && deathSound != null)
            {
                audioSource.PlayOneShot(deathSound);
            }
            Die();
        }
    }

    void Die()
    {
        isDead = true;
        Debug.Log("Treasure Bob has died!");

        // Εμφάνιση μηνύματος "Game Over"
        if (gameOverText != null)
        {
            gameOverText.SetActive(true);
        }

        // Πάγωμα του παιχνιδιού
        Time.timeScale = 0;

        // Ξεκινάμε την κορουτίνα για τερματισμό
        StartCoroutine(WaitAndQuit());
    }

    IEnumerator WaitAndQuit()
    {
        yield return new WaitForSecondsRealtime(delayBeforeQuit); // Αναμονή για καθορισμένα δευτερόλεπτα

        QuitGame();
    }

    private void QuitGame()
    {
        Debug.Log("Game is quitting...");
#if UNITY_EDITOR
        UnityEditor.EditorApplication.isPlaying = false; // Τερματισμός μέσα από το Unity Editor
#else
        Application.Quit(); // Πλήρης τερματισμός στο Build
#endif
    }
}
