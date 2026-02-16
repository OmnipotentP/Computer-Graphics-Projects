using UnityEngine;

public class TeleportExit : MonoBehaviour
{
    public Transform linkedExit; // Αναφορά στην άλλη έξοδο
    private bool isTeleporting = false; // Σημαία για τον έλεγχο teleport

    private void OnTriggerEnter(Collider other)
    {
        if (other.CompareTag("Player") && !isTeleporting) // Αν ο Bob περάσει και δεν είναι σε κατάσταση teleport
        {
            isTeleporting = true; // Ενεργοποίηση του flag για να αποτραπεί το διπλό teleport
            
            // Μεταφορά του Bob στη θέση της άλλης εξόδου
            Vector3 offset = other.transform.position - transform.position; // Για να διατηρείται η σχετική θέση
            other.transform.position = linkedExit.position + offset;

            // Επαναφορά του flag στην άλλη έξοδο
            TeleportExit linkedExitScript = linkedExit.GetComponent<TeleportExit>();
            if (linkedExitScript != null)
            {
                linkedExitScript.isTeleporting = true;
            }
        }
    }

    private void OnTriggerExit(Collider other)
    {
        if (other.CompareTag("Player")) // Όταν ο Bob φύγει από την έξοδο
        {
            isTeleporting = false; // Επαναφορά του flag
        }
    }
}
