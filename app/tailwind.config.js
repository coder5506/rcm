export default {
  content: ['./src/*.css', './src/*.tsx'],
  daisyui: {
    themes: ['light'],
  },
  plugins: [require('@tailwindcss/typography'), require('daisyui')],
}
