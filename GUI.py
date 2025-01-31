import customtkinter as ctk
import subprocess
import threading

class SpellCheckerApp:
    def __init__(self, root):
        self.root = root
        self.root.title("Correcto")
        self.root.geometry("850x700")
        self.root.resizable(True, True)

        ctk.set_appearance_mode("Light")
        ctk.set_default_color_theme("green")

        # Label for the input text area
        self.text_area_label = ctk.CTkLabel(
            root, 
            text="Enter text to check for spelling:", 
            font=('Times New Roman', 22)
        )
        self.text_area_label.grid(row=0, column=0, pady=10, padx=10, sticky='w')

        # Input text area
        self.text_area = ctk.CTkTextbox(
            root, 
            height=250, 
            width=600, 
            font=('Times New Roman', 18), 
            border_width=2, 
            corner_radius=10
        )
        self.text_area.grid(row=1, column=0, columnspan=2, pady=10, padx=10, sticky='ew')

        # Button to check spelling
        self.check_button = ctk.CTkButton(
            root, 
            text="Check Spelling", 
            command=self.start_spell_check_thread,  # Use a threaded function
            font=('Times New Roman', 20), 
            corner_radius=20, 
            width=150, 
            height=45
        )
        self.check_button.grid(row=2, column=0, pady=20, padx=10)

        # Button to clear the input and result areas
        self.clear_button = ctk.CTkButton(
            root, 
            text="Clear", 
            command=self.clear_text, 
            font=('Times New Roman', 20), 
            corner_radius=20, 
            width=150, 
            height=45
        )
        self.clear_button.grid(row=2, column=1, pady=20, padx=10)

        # Label for the result area
        self.result_text_label = ctk.CTkLabel(
            root, 
            text="Spell Check Results and Suggestions:", 
            font=('Times New Roman', 22)
        )
        self.result_text_label.grid(row=3, column=0, pady=10, padx=10, sticky='w')

        # Text area to display spell check results
        self.result_text = ctk.CTkTextbox(
            root, 
            height=250, 
            width=600, 
            font=('Times New Roman', 18), 
            state="disabled", 
            border_width=2, 
            corner_radius=10
        )
        self.result_text.grid(row=4, column=0, columnspan=2, pady=10, padx=10, sticky='ew')

        # Configure grid to allow resizing of widgets
        root.grid_rowconfigure(1, weight=1)
        root.grid_rowconfigure(4, weight=1)
        root.grid_columnconfigure(0, weight=1)
        root.grid_columnconfigure(1, weight=1)

    # Method to start the spell-checking process in a separate thread
    def start_spell_check_thread(self):
        input_text = self.text_area.get("1.0", "end-1c")  # Get all text from the input area

        if not input_text.strip():
            ctk.CTkMessagebox.show_warning("Input Error", "Please enter some text to check.")
            return

        # Disable the "Check Spelling" button while processing
        self.check_button.configure(state="disabled")

        # Start the spell-checking process in a separate thread
        threading.Thread(target=self.check_spelling, args=(input_text,), daemon=True).start()

    # Method to check spelling using the external spell checker program
    def check_spelling(self, input_text):
        try:
            result = subprocess.run(
                ["./spellchecker.exe"], 
                input=input_text, 
                capture_output=True, 
                text=True, 
                timeout=30
            )

            # Check if the spell checker program encountered an error
            if result.returncode != 0:
                ctk.CTkMessagebox.show_error("Spell Checker Error", f"Error: {result.stderr}")
                return

            # Display the spell checker output
            output = result.stdout
            self.result_text.configure(state="normal")
            self.result_text.delete("1.0", "end")

            # Process and display each line of the output
            output_lines = output.splitlines()
            for line in output_lines:
                if "Suggestions for" in line:
                    self.result_text.insert("end", line + "\n")
                elif line.startswith("-"):
                    self.result_text.insert("end", "    " + line + "\n")
                else:
                    self.result_text.insert("end", line + "\n")

            # If no suggestions were found, add a message
            if "Suggestions for" not in output:
                self.result_text.insert("end", "\nNo suggestions found for any misspelled word.\n")

            self.result_text.configure(state="disabled")

        except subprocess.TimeoutExpired:
            ctk.CTkMessagebox.show_error("Timeout Error", "The spell checker took too long to respond.")
        except Exception as e:
            ctk.CTkMessagebox.show_error("Error", f"An unexpected error occurred: {str(e)}")
        finally:
            # Re-enable the "Check Spelling" button after processing
            self.check_button.configure(state="normal")

    # Method to clear the input and result areas
    def clear_text(self):
        self.text_area.delete("1.0", "end")
        self.result_text.configure(state="normal")
        self.result_text.delete("1.0", "end")
        self.result_text.configure(state="disabled")


# Set up the main application window
root = ctk.CTk()

# Create an instance of the SpellCheckerApp
app = SpellCheckerApp(root)

# Start the event loop for the GUI
root.mainloop()
