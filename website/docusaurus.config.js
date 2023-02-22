// @ts-check
// Note: type annotations allow type checking and IDEs autocompletion

const lightCodeTheme = require('prism-react-renderer/themes/github');
const darkCodeTheme = require('prism-react-renderer/themes/dracula');

/** @type {import('@docusaurus/types').Config} */
const config = {
  title: 'Uprising',
  tagline: 'Software and hardware documentation for the robot artist project',
  favicon: 'img/favicon.ico',

  // Set the production url of your site here
  url: 'https://your-docusaurus-test-site.com',
  // Set the /<baseUrl>/ pathname under which your site is served
  // For GitHub pages deployment, it is often '/<projectName>/'
  baseUrl: '/',

  // GitHub pages deployment config.
  // If you aren't using GitHub pages, you don't need these.
  organizationName: 'hoolymama', // Usually your GitHub org/user name.
  projectName: 'uprising', // Usually your repo name.

  onBrokenLinks: 'throw',
  onBrokenMarkdownLinks: 'warn',

  // Even if you don't use internalization, you can use this field to set useful
  // metadata like html lang. For example, if your site is Chinese, you may want
  // to replace "en" with "zh-Hans".
  
  i18n: {
    defaultLocale: 'en',
    locales: ['en'],
  },

  presets: [
    [
      'classic',
      /** @type {import('@docusaurus/preset-classic').Options} */
      ({
        docs: {
          sidebarPath: require.resolve('./sidebars.js'),
        },
        theme: {
          customCss: require.resolve('./src/css/custom.css'),
        },
      }),
    ],
  ],

  themeConfig:
    /** @type {import('@docusaurus/preset-classic').ThemeConfig} */
    ({
      // Replace with your project's social card
      image: 'img/docusaurus-social-card.jpg',
      navbar: {
        title: 'Uprising',
        logo: {
          alt: 'Uprising Logo',
          src: 'img/logo.png',
        },
        
        items: [
          {
            type: 'doc',
            position: 'left',
            docId: 'software-intro',
            label: 'Software',
          },
          {
            // type: 'docSidebar',
            type: 'doc',
            position: 'left',
            docId: 'hardware-intro',
            // sidebarId: 'api',
            label: 'Hardware',
          },
          {
            // type: 'docSidebar',
            type: 'doc',
            position: 'left',
            docId: 'resources-intro',
            // sidebarId: 'api',
            label: 'Resources',
          },


          // {
          //   type: 'doc',
          //   docId: 'intro',
          //   position: 'left',
          //   label: 'Software',
          // },
          // {to: '/hardware', label: 'Hardware', position: 'left'},
          // {to: '/blog', label: 'Blog', position: 'left'},
          {
            href: 'https://github.com/robandnick/uprising',
            label: 'GitHub',
            position: 'right',
          },
        ],
      },
      footer: {
        style: 'dark',
        links: [
          {
            title: 'Docs',
            items: [
              {
                label: 'Software',
                to: '/docs/software-intro',
              },
              {
                label: 'Hardware',
                to: '/docs/hardware-intro',
              },
              {
                label: 'Resources',
                to: '/docs/resources-intro',
              },
            ],
          },
          {
            title: 'Community',
            items: [
              {
                label: 'Website',
                href: 'https://www.robandnick.com/',
              },
              {
                label: 'Twitter',
                href: 'https://twitter.com/RobandNick',
              },
            ],
          },
          {
            title: 'More',
            items: [
              {
                label: 'GitHub',
                href: 'https://github.com/robandnick/uprising',
              },
            ],
          },
        ],
        copyright: `Copyright © ${new Date().getFullYear()} Rob & Nick Ltd.`,
      },
      prism: {
        theme: lightCodeTheme,
        darkTheme: darkCodeTheme,
      },
    }),
};

module.exports = config;
