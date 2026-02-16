using UnityEngine;
using UnityEngine.UI;

public class ScoreManager : MonoBehaviour
{
    public static ScoreManager Instance { get; private set; }

    public Text scoreText; // UI για την εμφάνιση του σκορ
    public GameObject winText; // Αναφορά στο μήνυμα "You Won!"
    public AudioClip winSound; // Ήχος για τη νίκη
    private AudioSource audioSource; // AudioSource για αναπαραγωγή του ήχου
    private int score = 0;
    private bool gameEnded = false; // Έλεγχος για το αν έχει τελειώσει το παιχνίδι
    public int winningScore = 500; // Σκορ για νίκη

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
        UpdateScoreText();
        audioSource = GetComponent<AudioSource>(); // Παίρνουμε το AudioSource από το GameObject
        if (audioSource == null)
        {
            // Αν δεν υπάρχει AudioSource, δημιουργούμε ένα
            audioSource = gameObject.AddComponent<AudioSource>();
        }
    }

    public void AddScore(int points)
    {
        if (gameEnded) return; // Αν έχει τελειώσει το παιχνίδι, αγνόησε το σκορ
        score += points;
        UpdateScoreText();

        if (score >= winningScore)
        {
            WinGame();
        }
    }

    private void UpdateScoreText()
    {
        if (scoreText != null)
        {
            scoreText.text = "Score: " + score;
        }
    }

    private void WinGame()
    {
        gameEnded = true;
        Debug.Log("Congratulations, You Won!");
        
        // Παίξιμο ήχου νίκης
        if (winSound != null && audioSource != null)
        {
            audioSource.PlayOneShot(winSound);
        }

        // Εμφάνιση του μηνύματος νίκης
        if (winText != null)
        {
            winText.SetActive(true);
        }

        // Πάγωμα του παιχνιδιού
        Time.timeScale = 0;

        // Τερματισμός μετά από λίγη ώρα
        StartCoroutine(WaitAndQuit());
    }

    private System.Collections.IEnumerator WaitAndQuit()
    {
        yield return new WaitForSecondsRealtime(4.0f); // Αναμονή για 4 δευτερόλεπτα 

        // Τερματισμός παιχνιδιού
#if UNITY_EDITOR
        UnityEditor.EditorApplication.isPlaying = false; // Τερματισμός στο Unity Editor
#else
        Application.Quit(); // Τερματισμός στο Build
#endif
    }
}
