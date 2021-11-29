<!-- <script>
  import { onMount } from "svelte";
  let temp = "temp here";
  onMount(async () => {
    await fetch("http://192.168.0.240:8000/data")
      .then((r) => r.text())
      .then((data) => {
        temp = data;
      });
  });
  console.log(temp);
</script> -->
<script>
  import { onMount } from "svelte";
  let characterName;

  onMount(async () => {
    const response = await fetch("http://192.168.0.240:8000/data");
    const character = await response.json();
    characterName = character.name;
  });
</script>

<main>
  <h1>Hello {characterName}!</h1>
  <p>
    Visit the <a href="https://svelte.dev/tutorial">Svelte tutorial</a> to learn
    how to build Svelte apps.
  </p>
</main>

<style>
  main {
    text-align: center;
    padding: 1em;
    max-width: 240px;
    margin: 0 auto;
  }

  h1 {
    color: #ff3e00;
    text-transform: uppercase;
    font-size: 4em;
    font-weight: 100;
  }

  @media (min-width: 640px) {
    main {
      max-width: none;
    }
  }
</style>
