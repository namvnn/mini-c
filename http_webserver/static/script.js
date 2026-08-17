function copy(codeblock, button) {
  button.addEventListener("click", async () => {
    try {
      await navigator.clipboard.writeText(codeblock.textContent);
      button.textContent = "copied";
      setTimeout(() => {
        button.textContent = "copy";
      }, 2000);
    } catch (error) {
      console.error("Failed to copy:", error);
      button.textContent = "failed";
      setTimeout(() => {
        button.textContent = "copy";
      }, 2000);
    }
  });
}

const n = 3;
for (let i = 1; i <= n; i++) {
  const codeblock = document.getElementById(`cb${i}`);
  const button = document.getElementById(`cb${i}-copy`);
  copy(codeblock, button);
}
