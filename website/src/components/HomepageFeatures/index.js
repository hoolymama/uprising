import React from 'react';
import clsx from 'clsx';
import styles from './styles.module.css';

const FeatureList = [
  {
    title: 'My first painting',
    imgsrc: '/img/firstcircle.png',
    description: (
      <>
        Uprising was designed from the ground up to be easily installed to get the robot up and running quickly.
      </>
    ),
  },
  {
    title: 'Portrait of Picasso',
    imgsrc: '/img/picasso.png',
    description: (
      <>
        Customize the robot's capabilities. Uprising's modular design allows you to implement new artists styles with ease.
      </>
    ),
  },
  {
    title: 'Sunflowers',
    imgsrc: '/img/sunflowers.png',
    description: (
      <>
        Uprising lets you focus on your creativity. It's easy to use and easy to extend. 
      </>
    ),
  },
];

function Feature({imgsrc, title, description}) {
  return (
    <div className={clsx('col col--4')}>
      <div className="text--center">
        <img className={styles.featureImage} src={imgsrc} alt={title} />
      </div>
      <div className="text--center padding-horiz--md">
        <h3>{title}</h3>
        <p>{description}</p>
      </div>
    </div>
  );
}

export default function HomepageFeatures() {
  return (
    <section className={styles.features}>
      <div className="container">
        <div className="row">
          {FeatureList.map((props, idx) => (
            <Feature key={idx} {...props} />
          ))}
        </div>
      </div>
    </section>
  );
}
