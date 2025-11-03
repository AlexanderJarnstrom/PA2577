class Statistic {
  #averageTotal = 0;
  #averageMatch = 0;
  #totalTotal = 0;
  #totalMatch = 0;
  #values = [];
  #cap = 0;

  constructor(cap) {
    this.#cap = cap;
  }

  get averageTotal() { return this.#averageTotal; }
  get averageMatch() { return this.#averageMatch; }
  get size() { return this.#values.length; }

  addTime(timer) {
    const total = Number(timer['total']);
    const match = Number(timer['match']);

    this.#values.push(timer);
    this.#totalTotal += total;
    this.#totalMatch += match;

    if (this.#cap != -1 && this.#values.length > this.#cap) {
      let old = this.#values.shift();
      this.#totalTotal -= Number(old['total']);
      this.#totalMatch -= Number(old['match']);
    }

    this.#averageTotal = this.#totalTotal / this.#values.length;
    this.#averageMatch = this.#totalMatch / this.#values.length;
  }

  printStats() {
    console.log("Average for " + this.#cap);
    console.log("\t- total: " + this.#averageTotal);
    console.log("\t- match: " + this.#averageMatch);
  }
}

module.exports = Statistic;
