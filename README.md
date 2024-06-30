# Mahatma Gandhi Q&A System

Welcome to the Mahatma Gandhi Question-Answering System! This project answers questions related to the life and thoughts of Mahatma Gandhi using a substantial corpus of texts detailing his life.

## Features

- **Question-Answering System**: Answers questions about Mahatma Gandhi's life and ideology.
- **Text Pre-processing**: Utilizes the Porter Stemming Algorithm.
- **Paragraph Ranking**: Scores paragraphs based on relevance to a query using TF/IDF via Hashmaps.
- **Top Paragraph Extraction**: Uses a custom Heap data structure to extract the top 5 paragraphs.
- **Natural Language Response**: Constructs a coherent response using the ChatGPT API.

## Setup

Follow these steps to set up and run the project:

1. **Download the Repository**
    ```bash
    git clone https://github.com/yourusername/mahatma-gandhi-qna.git
    cd mahatma-gandhi-qna
    ```

2. **Download the Corpus Folder**
    - Download the "corpus" folder from the given drive link: [Drive Link]
    - Drag and drop the "corpus" folder into the `mahatma-gandhi-qna` repository folder.

3. **Add ChatGPT API Key**
    - Open `mahatma_qna.cpp`.
    - Add your ChatGPT API key in the specified section.

4. **Compile and Run**
    - Ensure you have C++20 installed.
    ```bash
    g++ -std=c++20 mahatma_qna.cpp -o mahatma_qna
    ./mahatma_qna
    ```

## Usage

Once the setup is complete, you can start querying the system for information about Mahatma Gandhi. Simply run the program and input your questions when prompted.

---

Enjoy exploring the life and thoughts of Mahatma Gandhi through this Q&A system!
